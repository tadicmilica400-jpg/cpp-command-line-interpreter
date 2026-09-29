#include "HeadCommand.h"
#include "Error.h"
#include <iostream>
#include <sstream>

// Ispisuje prvih n linija sa standardnog ulaza ili iz fajla
void HeadCommand::execute(std::istream& in, std::ostream& out) {
    std::ostringstream buffer;
	// Cita ceo ulaz u buffer
    buffer << in.rdbuf();
    std::istringstream tempStream(buffer.str());
    std::string line;
    int printed = 0;
    std::ostringstream oss;
    bool last = false;
	// Ispisuje prvih count linija
    while (printed < count && std::getline(tempStream, line)) {
        if (count == printed + 1) last = true;
        out << line << std::endl;
        printed++;
        oss << line;
        if (!last) {
            oss << std::endl;
        }
    }
    setText(oss.str());
}
