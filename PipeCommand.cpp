#include "PipeCommand.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

void PipeCommand::execute(std::istream& in, std::ostream& out) {
    try {
        // Ukoliko nema argumenata nije moguce izvrsiti
        if (commands.empty()) return;

        std::istream* curIn = &in;
        std::ostream* curOut = &out;
        std::unique_ptr<std::stringstream> bufferIn;
        std::unique_ptr<std::stringstream> bufferOut;
        std::unique_ptr<std::istream> fileStreamIn;
        std::unique_ptr<std::ofstream> fileStreamOut;
        // Proveravamo da li je zadat ulazni tok prve komande
        if (!inputFile.empty()) {
            fileStreamIn = std::make_unique<std::ifstream>(inputFile);
            curIn = fileStreamIn.get();
        }
        // Izvrsavamo svaku podkomandu pipe-a 
        for (size_t i = 0; i < commands.size(); i++) {
            bool isLast = (i == commands.size() - 1);

            // Proveravamo da li slucajno postoji zadati ulazni ili izlazni tok komande sto nije dozvoljeno
            // Ako nije, stavljamo izlazni tok prethodno izvrsene komande na ulazni tok sledece
            if (!isLast) {
                bufferOut = std::make_unique<std::stringstream>();
                if (i != 0) Error::checkFileNotExists(commands[i]->getInputFile());
                Error::checkFileNotExists(commands[i]->getOutputFile());
                commands[i]->execute(*curIn, *bufferOut);

                bufferIn = std::make_unique<std::stringstream>(commands[i]->getText());
                curIn = bufferIn.get();
            }
            // Proveravamo da li postoji izlazni tok poslednje komande
            else {
                if (!outputFile.empty()) {
                    Error::checkFileExists(outputFile);
                    if (append)
                        fileStreamOut = std::make_unique<std::ofstream>(outputFile, std::ios::app);
                    else
                        fileStreamOut = std::make_unique<std::ofstream>(outputFile, std::ios::trunc);
                    curOut = fileStreamOut.get();
                }
                commands[i]->execute(*curIn, *curOut);
            }
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("pipe: ") + e.what());
	}
}
