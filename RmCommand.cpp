#include "RmCommand.h"
#include <filesystem>
#include <iostream>
#include <fstream>

//Brisemo fajl zadat kao argument rm komande
void RmCommand::execute(std::istream& in, std::ostream& out) {
    if (std::remove(filename.c_str()) == 0) {
        out << "File '" << filename << "' removed.\n";
    }
    else {
        out << "rm: cannot remove '" << filename << "'" << std::endl;
    }
}
