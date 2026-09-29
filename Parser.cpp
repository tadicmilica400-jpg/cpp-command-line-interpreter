#include <sys/stat.h>
#include "Parser.h"
#include <sstream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

// Parsira liniju komandnog interfejsa u ParsedCommand strukturu
ParsedCommand Parser::parse(const std::string& line) {
    ParsedCommand cmd;
	// Prpvera leksickih gresaka, tj. duzine linije i nedozvoljenih karaktera
    Error::checkLineLength(line);
    Error::unexpectedCharacters(line);
    cmd.raw = line;

    std::istringstream iss(line);
    std::string token;
    bool firstToken = true;

	// Provera da li je u pitanju PIPE komanda
    if (line.find('|') != std::string::npos) {
        cmd.name = "pipe";
        try {
            ;
            std::stringstream ss(line);
            std::string segment;
            bool first = true;

            while (std::getline(ss, segment, '|')) {
                segment.erase(0, segment.find_first_not_of(" \t"));
                segment.erase(segment.find_last_not_of(" \t") + 1);

				// Rekurzivno parsiranje svake podkomande
                ParsedCommand sub = Parser::parse(segment);
                cmd.subcommands.push_back(std::move(sub));
                cmd.args.push_back(segment);
            }
            return cmd;
        }
        catch (const std::exception& e) {
            throw std::invalid_argument(std::string("pipe: ") + e.what());
		}
    }
	// Parsiranje ostalih komandi
    try {
        // Postepeno citanje linije
        while (iss >> token) {

			// Uzimanje imena komande
            if (firstToken) {
                firstToken = false;
                cmd.name = token;
                if (cmd.name == "date" || cmd.name == "time") cmd.args.push_back(token);
                continue;
            }

            // Opcije
            if (!token.empty() && token[0] == '-') {
                cmd.options.push_back(token);
            }

            // Redirekcije
            else if (token == "<" || token == ">" || token == ">>" ||
                token[0] == '<' || token[0] == '>') {

                std::string file;

				// Slucaj kada je razdvojeno: < file, > file, >> file
                if (token == "<" || token == ">" || token == ">>") {
                    if (iss >> file) {
                        if (token == "<") {
                            if(cmd.inputFile=="") cmd.inputFile = file;
							else throw std::invalid_argument("Vise ulaznih redirekcija.");
                            if (cmd.name == "touch" || cmd.name == "rm" || cmd.name == "truncate") {
                                throw std::invalid_argument(": redirection not allowed");
                            }
                        }
                        else {
                            if (cmd.outputFile == "") cmd.outputFile = file;
                            else throw std::invalid_argument("Vise ulaznih redirekcija.");
                            cmd.append = (token == ">>");
                            if (cmd.name == "touch" || cmd.name == "rm" || cmd.name == "truncate") {
								throw std::invalid_argument(": redirection not allowed");
                            }
                        }
                    }
                }
                // Sluxaj "zalepljeno": <file, >file, >>file
                else {
                    if (token[0] == '<') {
                        file = token.substr(1);
                        if (cmd.inputFile == "") cmd.inputFile = file;
                        else throw std::invalid_argument("Vise ulaznih redirekcija.");
                        if (cmd.name == "touch" || cmd.name == "rm" || cmd.name == "truncate") {
                            throw std::invalid_argument(": redirection not allowed");
                        }
                    }
                    else if (token.rfind(">>", 0) == 0) {
                        file = token.substr(2);
                        if (cmd.outputFile == "") cmd.outputFile = file;
                        else throw std::invalid_argument("Vise ulaznih redirekcija.");
                        cmd.append = true;
                        if (cmd.name == "touch" || cmd.name == "rm" || cmd.name == "truncate") {
                            throw std::invalid_argument(": redirection not allowed");
                        }
                    }
                    else if (token[0] == '>') {
                        file = token.substr(1);
                        if (cmd.outputFile == "") cmd.outputFile = file;
                        else throw std::invalid_argument("Vise ulaznih redirekcija.");
                        cmd.append = false;
                        if (cmd.name == "touch" || cmd.name == "rm" || cmd.name == "truncate") {
                            throw std::invalid_argument(": redirection not allowed");
                        }
                    }
                }
            }
			// Redirekcije (ponovo, za slucaj da je bilo razdvojeno)
            else if (token == "<" || token == ">" || token == ">>") {
                std::string file;
                if (iss >> file) {
                    if (token == "<")
                    {
                        if (cmd.inputFile == "") cmd.inputFile = file;
                        else throw std::invalid_argument("Vise ulaznih redirekcija.");
                        if (cmd.name == "touch" || cmd.name == "rm" || cmd.name == "truncate") {
                            throw std::invalid_argument(": redirection not allowed");
                        }
                    }
                    if (token != "<") {
                        if (cmd.outputFile == "") cmd.outputFile = file;
                        else throw std::invalid_argument("Vise ulaznih redirekcija.");
                        cmd.append = (token == ">>");
                        if (cmd.name == "touch" || cmd.name == "rm" || cmd.name == "truncate") {
                            throw std::invalid_argument(": redirection not allowed");
						}
                    }
                }
            }
			// Uzimanje argumenata ukoliko su dati u navodnicima
            else if (!token.empty() && token.front() == '"') {
                std::string txt;
                if (token.size() > 1) {
                    txt = token.substr(1); // deo posle prvog "
                    if (!txt.empty() && txt.back() == '"') {
                        txt.pop_back();
                        cmd.args.push_back(txt);
                        continue;
                    }
                }
                std::string rest;
                std::getline(iss, rest, '"');
				// Za tr komandu, spajanje argumenata sa razmakom
                if (cmd.name == "tr") txt += (txt.empty() ? "" : " ") + rest;
                else txt += rest;
                cmd.args.push_back(txt);
            }
            else {
				// Provera da li fajl postoji ako je komanda takva da radi sa fajlovima
                struct stat buffer;
                if (stat(token.c_str(), &buffer) == 0) {
                    cmd.inputFile = token;
                    if (cmd.name == "touch" || cmd.name == "rm" || cmd.name == "truncate") {
                        cmd.args.push_back(token);
                        cmd.inputFile = "";
                    }
                }
                else {
                    if (cmd.name == "touch" || cmd.name == "rm" || cmd.name == "truncate") {
						cmd.args.push_back(token);  
                    }
                    else throw std::invalid_argument(": argument '" + token + "' has no quotes");
                }
            }
			// Provera da li je komanda ispravnog formata, tj da li je redirekcija na pravom mestu i da li ima previse opcija
            Error::inputRedirectionError(cmd.args, cmd.inputFile);
            Error::outputRedirectionError(cmd.args, cmd.outputFile);
            if(cmd.name == "wc" || cmd.name == "head") Error::checkOptionsCount(cmd.name, cmd.options, 1);
        }
        // Dodatne provere i prilagodjavanja za specijalne komande
        if (cmd.name == "tr") {
            if (!cmd.args.empty()) {
				// Tr ima posebnu formu: tr <what> <with> [text...] i neophodno je 
				// da se izdvoje what i with iz argumenata
				// Provera da li je what ispravan
                if (!cmd.inputFile.empty()) {
                    cmd.what = cmd.args[0];
                    cmd.with = (cmd.args.size() > 1 ? cmd.args[1] : "");
                    cmd.args.clear();
                }
				// Ukoliko ima vise od 2 argumenta, prvi je what, drugi with, a ostali su tekst
                else if (cmd.args.size() > 2) {
                    cmd.what = cmd.args[0];
                    cmd.with = cmd.args[1];
                    std::vector<std::string> remaining;
                    for (size_t i = 2; i < cmd.args.size(); ++i)
                        remaining.push_back(cmd.args[i]);
                    cmd.args = remaining;
                }
				// Provera da li ima tacno 2 argumenta
                else if (cmd.args.size() == 2) {
                    cmd.what = cmd.args[0];
                    cmd.with = cmd.args[1];
                    cmd.args.clear(); 
                }
				// Provera da li ima samo jedan argument
                else {
                    cmd.what = cmd.args[0];
                    cmd.with = "";
                    cmd.args.clear();
                }
            }
			Error::chechWhat(cmd.name, cmd.what);
        }
        // Dodatna provera za batch komandu
        if (cmd.name == "batch") {
            std::vector<std::string> lines;

            // Proveravamo da li je zadat ulazni tok
            if (!cmd.inputFile.empty()) {
                Error::checkFileExists(cmd.inputFile);
                std::ifstream ifs(cmd.inputFile);
                lines = readUntilEOF(ifs);
            }
            // Provera da li je batch u pipu i tada direktno u izvrsavanju pipe komande postavljamo ulazni i izlazni tok batcha
            else if (!cmd.subcommands.empty()) {
                for (auto& sc : cmd.subcommands) {
                    lines.push_back(sc.raw);
                }
            }
            // Stvaranje podkomandi batcha
            for (auto& l : lines) {
                if (l.empty()) continue;
                ParsedCommand sub = Parser::parse(l);
                if (sub.name != "echo" && sub.name != "wc" && sub.name != "head" && sub.name != "tr"
                    && sub.name != "date" && sub.name != "time" && sub.name != "prompt"
                    && sub.name != "rm" && sub.name != "truncate" && sub.name !="touch") {
                }
                else{
                    cmd.subcommands.push_back(std::move(sub));
                    cmd.args.push_back(l);
                }

            }
        }
        if (!cmd.inputFile.empty() && (cmd.name!="batch" && !cmd.args.empty())) {
            throw std::invalid_argument(cmd.name + ": command has both arguments and input stream");
        }
        if (cmd.name != "wc" && cmd.name != "head") {
            if (cmd.options.size() > 0) {
                throw std::invalid_argument(": command does not take options");
			}
        }
        return cmd;
    }
    catch (const std::exception& e) {
        throw std::invalid_argument(cmd.name + ": " + e.what());
	}
}

// Funkcija koja radi citanje do kraja ulaza ukoliko je neophodno
std::vector<std::string> Parser::readUntilEOF(std::istream& in) {
    std::vector<std::string> lines;
    std::string line;
    while (true) {
        if (!std::getline(in, line)) break;
        if (line == "EOF") break;          
        lines.push_back(line);
    }
	in.clear();
    return lines;
}

