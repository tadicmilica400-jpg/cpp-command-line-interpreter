#include "BatchCommand.h"
#include "Parser.h"
#include "CommandBuilder.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

// Izvrsava seriju komandi procitanih iz fajla ili standardnog ulaza
void BatchCommand::execute(std::istream& in, std::ostream& out) {
    std::vector<std::string> lines;

	// Ako je naveden ulazni fajl, cita iz njega, inace cita iz standardnog ulaza
    if (!inputFile.empty()) {
        std::ifstream ifs(inputFile);
		Error::checkFileExists(inputFile);
        lines = Parser::readUntilEOF(ifs);
    }
	//Ukoliko su navedeni argumenti, prvi argument tretira kao ulazni fajl
    else if (!args.empty()) {
        std::ifstream ifs(args[0]);
        Error::checkFileExists(inputFile);
        lines = Parser::readUntilEOF(ifs);
    }
	//Ukoliko nisu navedeni ni ulazni fajl, ni argumenti, cita iz standardnog ulaza
    else {
        lines = Parser::readUntilEOF(in);
    }
	// Izvrsava svaku komandu iz batch fajla
    for (auto& l : lines) {
        if (l.empty()) continue;
        ParsedCommand sub = Parser::parse(l);
        auto cmd = CommandBuilder::create(sub);
        if (!cmd) {
            std::cout << "batch: command not found: " << sub.name << std::endl;
            continue;
        }
        std::unique_ptr<std::istream> fileStreamIn;
        std::unique_ptr<std::ofstream> fileStreamOut;
        std::istringstream textStream;
        std::istream* in = &std::cin;
        std::ostream* out = &std::cout;
        // Ako je komanda imala da ulazni fajl, otvara ga i koristi kao ulaz
        if (!sub.inputFile.empty()) {
            fileStreamIn = std::make_unique<std::ifstream>(sub.inputFile);
            in = fileStreamIn.get();
        }
        // Ako komanda nema argumente, cita iz standardnog ulaza
        else if (sub.args.empty()) {
            auto lines = Parser::readUntilEOF(std::cin);
            std::ostringstream oss;
            for (auto& l : lines) oss << l << "\n";
            sub.in = std::make_unique<std::istringstream>(oss.str());
            in = sub.in.get();
        }
        // Ako je komanda imala izlazni fajl, otvara ga i koristi kao izlaz
        if (!sub.outputFile.empty()) {
            if (sub.append)
                fileStreamOut = std::make_unique<std::ofstream>(sub.outputFile, std::ios::app);
            else
                fileStreamOut = std::make_unique<std::ofstream>(sub.outputFile, std::ios::trunc);

            out = fileStreamOut.get();
        }
        // Proverava da li komanda postoji i izvrsava je
        try {
			Error::checkCommandExists(sub.name);
            cmd->execute(*in, *out);
        }
        catch (const std::exception& e) {
            std::cerr << "batch: " << e.what() << std::endl;
		}
    }
}
