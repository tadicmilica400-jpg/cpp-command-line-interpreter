#include "WCCommand.h"
#include "Error.h"
#include <sstream>
#include <fstream>
#include <cctype>

// Funkcija koja izvrsava komandu za brojanje reci ili karaktera u zavisnosti od moda
void WCCommand::execute(std::istream& in, std::ostream& out) {
    try {
        std::string content;
        // Ako su prosledjeni argumenti, koristi ih kao ulazni tekst
        if (!args.empty()) {
			Error::checkArgumentsCount("wc", args, 1);
            for (size_t i = 0; i < args.size(); i++) {
                if (i > 0) content += " ";
                content += args[i];
            }
        }
        // Ako je postavljen ulazni tok iz fajla, koristi ga
        else if (inStream) {
            std::ostringstream oss;
            oss << inStream->rdbuf();
            content = oss.str();
        }
        // U suprotnom, koristi standardni ulazni tok
        else {
            std::ostringstream oss;
            oss << in.rdbuf();
            content = oss.str();
        }
        // Brojanje reci ili karaktera u zavisnosti od moda
        int count = 0;
        if (mode == Mode::WORDS) {
            std::istringstream iss(content);
            std::string w;
            while (iss >> w) ++count;
        }
        else if (mode == Mode::CHARS) {
            for (char c : content) {
                if (c == '"') continue;
                ++count;
            }
        }
        out << count << std::endl;
        setText(std::to_string(count));
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("wc: ") + e.what());
    }
}