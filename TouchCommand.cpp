#include <sys/stat.h>
#include "TouchCommand.h"
#include "Error.h"
#include <fstream>
#include <stdexcept>

// Komanda koja stvara prazan fajl ukoliko on ne vec postoji
void TouchCommand::execute(std::istream& in, std::ostream& out) {
    if (filename.empty()) {
        out << "touch: missing filename" << std::endl;
        return;
    }
    if (filename.find('.') == std::string::npos) {
        filename += ".txt";
    }
    struct stat buffer;
    if (stat(filename.c_str(), &buffer) == 0) {
		throw std::invalid_argument("touch: file '" + filename + "' already exists");
    }
    try {
        std::ofstream ofs(filename);
        Error::checkFileExists(filename);
    }
    catch (const std::exception& e) {
        throw std::invalid_argument( std::string("touch: ") + e.what());
	}
}

