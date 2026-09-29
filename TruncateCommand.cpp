#include "TruncateCommand.h"
#include "Error.h"
#include <fstream>
#include <iostream>

// Funkcija koja prazni fajl 
void TruncateCommand::execute(std::istream& in, std::ostream& out) {
    try {
        Error::checkFileExists(filename);
        std::ofstream ofs(filename, std::ios::trunc);
        if (!ofs) {
            out << "truncate: can't open the file " << filename << std::endl;
            return;
        }
        ofs.close();
        out << "truncate: file \"" << filename << "\" is empty now" << std::endl;
    }
    catch (const std::exception& e) {
        out << "truncate: couldn't empty file " << filename << ": " << e.what() << std::endl;
    }
}