#include "Interpreter.h"
#include "CommandBuilder.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <memory>
#include <stdexcept>

// Glavna petlja komandnog interfejsa
void Interpreter::run() {
    std::string line;
    while (true) {
		// Ucitava liniju sa standardnog ulaza
        std::cout << prompt;
        if (!std::getline(std::cin, line)) break;
		// Preskace prazne linije
        try {
            ParsedCommand pcmd = Parser::parse(line);
            auto cmd = CommandBuilder::create(std::move(pcmd));
			// Postavlja ulazni i izlazni stream
            std::unique_ptr<std::istream> fileStreamIn;
            std::unique_ptr<std::ofstream> fileStreamOut;
            std::istringstream textStream;
            std::istream* in = &std::cin;
			std::ostream* out = &std::cout; 
			// Provera da li je naveden ulazni fajl
            if (!pcmd.inputFile.empty()) {
                fileStreamIn = std::make_unique<std::ifstream>(pcmd.inputFile);
                in = fileStreamIn.get();
            }
			// Provera da li komanda nema argumente i u tom slucaju cita iz standardnog ulaza
            else if (pcmd.args.empty()) {
                auto lines = Parser::readUntilEOF(std::cin);
                std::ostringstream oss;
                for (auto& l : lines) oss << l << "\n";
                pcmd.in = std::make_unique<std::istringstream>(oss.str());
                in = pcmd.in.get();
            }
			// Provera da li je naveden izlazni fajl
            if (!pcmd.outputFile.empty()) {
                if (pcmd.append)
                    fileStreamOut = std::make_unique<std::ofstream>(pcmd.outputFile, std::ios::app);
                else
                    fileStreamOut = std::make_unique<std::ofstream>(pcmd.outputFile, std::ios::trunc);

                out = fileStreamOut.get();
            }
			// Izvrsava komandu, a u parseru je provereno da li je komanda postojeca i ispravnog formata
            cmd->execute(*in, *out);
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}



