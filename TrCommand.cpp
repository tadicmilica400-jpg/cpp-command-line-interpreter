#include "TrCommand.h"
#include <sstream>

// Funkcija koja izvrsava zamenu ili brisanje karaktera u tekstu
void TrCommand::execute(std::istream& in, std::ostream& out) {
	// Pribavljanje ulaznog teksta
    std::string text;
    if (!args.empty()) {
        for (size_t i = 0; i < args.size(); ++i) {
            text += args[i];
        }
    }
    else {
        in.clear();
        in.seekg(0);
        std::ostringstream oss;
        oss << in.rdbuf();
        text = oss.str();
    }

    // Zamena ili brisanje
    std::string result = text;
    size_t pos = 0;
    while ((pos = result.find(what, pos)) != std::string::npos) {
        if (hasWith) {
            result.replace(pos, what.size(), with);
            pos += with.size();
        }
        else {
            result.erase(pos, what.size());
        }
    }
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }
    out << result << std::endl;
	setText(result);
}
