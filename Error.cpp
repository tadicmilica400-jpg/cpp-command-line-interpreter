#include <algorithm>
#include "Error.h"

// Provera maksimalne duzine linije
void Error::checkLineLength(const std::string& line) {
    if (line.length() > 512) {
        throw std::invalid_argument("Linija je duza od 512 znakova.");
    }
}

// Provera nedozvoljenih karaktera
void Error::unexpectedCharacters(const std::string& line) {
    std::set<char> allowed = { ' ', '\t', '|', '<', '>', '-', '.', '/', '\n'};
    bool errorFound = false;
    std::string pointer(line.size(), ' ');

    for (size_t i = 0; i < line.size(); ++i) {
        if ((line[i] >= 'a' && line[i] <= 'z') ||
            (line[i] >= 'A' && line[i] <= 'Z') ||
            (line[i] >= '0' && line[i] <= '9') ||
            allowed.find(line[i]) != allowed.end()) {
            continue;
        }
        else if (line[i] == '"') {
            size_t j = i + 1;
            while (j < line.size() && line[j] != '"') ++j;
            if (j == line.size()) {
                throw std::invalid_argument("Navodnik nije zatvoren.");
            }
            i = j;
        }
        else {
            pointer[i] = '^';
            errorFound = true;
        }
    }

    if (errorFound) {
        throw std::invalid_argument("Nedozvoljeni karakteri:\n" + line + "\n" + pointer);
    }
}

// Provera da li komanda postoji
void Error::checkCommandExists(const std::string& command) {
    std::set<std::string> commands = { "echo","prompt","time","date","touch","truncate","rm","wc","tr","head","batch","pipe"};
    if (commands.find(command) == commands.end()) {
        throw std::invalid_argument("Unknown command: " + command);
    }
}

// Provera broja argumenata
void Error::checkArgumentsCount(const std::string& command, const std::vector<std::string>& args, size_t expected) {
    if (args.size() != expected) {
        throw std::invalid_argument("Komanda " + command + " nije ispravnog formata.");
    }
}

void Error::chechWhat(const std::string& command, const std::string& what) {
    if (what=="") {
        throw std::invalid_argument("Komanda " + command + " nije ispravnog formata.");
    }
}

// Provera broja opcija
void Error::checkOptionsCount(const std::string& command, const std::vector<std::string>& options, size_t maxAllowed) {
    if (options.size() > maxAllowed) {
        throw std::invalid_argument("Komanda " + command + " ima previse opcija.");
    }
    if (options.size() < 1) {
        throw std::invalid_argument("Komanda " + command + " nema dovoljno opcija.");
    }
	// Provera za Head i Wc
    if (command == "head") {
        std::string opt = options[0];
        if (opt.rfind("-n", 0) != 0) { 
            throw std::invalid_argument("head: invalid option, expected -n<count>");
        }
        std::string numStr = opt.substr(2);
        if (numStr.empty() || numStr.size() > 5) {
            throw std::invalid_argument("head: invalid line count");
        }
        for (char c : numStr) {
            if (!isdigit(c)) {
                throw std::invalid_argument("head: line count is not a number");
            }
        }
    }
    if(command == "wc") {
        std::string opt = options[0];
        if (opt != "-c" && opt != "-w") {
            throw std::invalid_argument("wc: invalid option, expected -c or -w");
        }
	}
}

//Provera za Tr
void Error::checkTrForm(const std::string& command, const std::string& what) {
    if (what == "") {
        throw std::invalid_argument("Komanda " + command + " ima previse opcija.");
    }
}


// Provera redirekcija
void Error::inputRedirectionError(const std::vector<std::string>& args, const std::string& input) {
    if (std::count(args.begin(), args.end(), "<") > 1) {
        throw std::invalid_argument(input.empty() ? "Ulazna redirekcija na pogresnom mestu." : "Vise ulaznih redirekcija.");
    }
}

void Error::outputRedirectionError(const std::vector<std::string>& args, const std::string& output) {
    if (std::count(args.begin(), args.end(), ">") + std::count(args.begin(), args.end(), ">>") > 1) {
        throw std::invalid_argument(output.empty() ? "Izlazna redirekcija na pogresnom mestu." : "Vise izlaznih redirekcija.");
    }
}

// Provera da li fajl postoji
void Error::fileExist(std::ifstream& file) {
    if (!file.is_open()) {
        throw std::runtime_error("Fajl ne postoji ili nema pristup.");
    }
}

// Pomocna funkcija za prikaz greske na odredjenoj poziciji
void Error::printErrorWithPointer(const std::string& line, size_t pos, const std::string& msg) {
    std::cerr << msg << ":\n" << line << "\n";
    std::cerr << std::string(pos, ' ') << "^\n";
}
// Provera da li fajl postoji
void Error::checkFileExists(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        throw std::runtime_error("File does not exist: " + filename);
    }
    ifs.close();
}

void Error::checkFileExistsTouch(const std::string& filename) {
    std::ifstream ifs(filename);
    if (ifs.is_open()) {
        throw std::runtime_error("File alredy exists: " + filename);
    }
    ifs.close();
}

// Provera da li fajl ne postoji ako je tako potrebno
void Error::checkFileNotExists(const std::string& filename) {
	std::ifstream ifs(filename);
    if (ifs.is_open()) {
        throw std::runtime_error("File should not exist: " + filename);
	}
    ifs.close();
}

// Provera za prompt
void Error::checkPrompt(const std::string& prompt) {
    if (prompt.empty() || prompt.length() > 100) {
        throw std::invalid_argument("Invalid prompt");
    }
}
