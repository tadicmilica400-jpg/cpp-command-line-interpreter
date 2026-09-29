#include "EchoCommand.h"
#include "Error.h"  
#include <iostream>
#include <sstream>

// Ispisuje argumente na standardni izlaz ili u fajl, ili cita iz standardnog ulaza ako nema argumenata
void EchoCommand::execute(std::istream& in, std::ostream& out) {
    try {
		// Ako nema argumenata, cita iz standardnog ulaza
        if (args.empty()) {
            std::ostringstream oss;
            std::string line;
            bool first = true;
            while (std::getline(in, line)) {
                if (!first) oss << '\n';
                oss << line;
                first = false;
            }
            std::string content = oss.str();

            if (content.size() >= 2 && content[content.size() - 2] == '\n') {
                content.erase(content.size() - 2);
            }
			// Ako je procitano nesto, dodaj to kao argument
            if (!content.empty()) {
                args.push_back(content);
            }
            if(content.empty()) {
                args.push_back("");
            }
        }
        Error::checkArgumentsCount("echo", args, 1);

        std::ostringstream oss;
        for (size_t i = 0; i < args.size(); ++i) {
            out << args[i];
            oss << args[i];
        }        if (getOutputFile().empty()) out << std::endl;
        setText(oss.str());
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("echo: ") + e.what());
	}
}