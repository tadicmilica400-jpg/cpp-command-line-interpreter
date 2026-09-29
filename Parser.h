#ifndef Parser_h
#define Parser_h

#include "Error.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <memory>

// Struktura koja sadrzi sve potrebne informacije o jednoj komandi
struct ParsedCommand {
    std::string raw;
    std::string name;
    std::vector<std::string> args;
    std::vector<std::string> options;
    std::string text;
    std::string inputFile="";
    std::string outputFile;
    std::string with;
    std::string what;
    bool append = false;
    std::vector<ParsedCommand> subcommands;
    std::unique_ptr<std::istringstream> in;
};

// Klasa koja parsira liniju komandne linije i pravi ParsedCommand strukturu i takodje ujedno proverava leksicke i sintaksne greske
//Takodje i proverava da li fajlovi postoje kod redirekcija i da li je sve ispravno napisano
class Parser {
public:

    static ParsedCommand parse(const std::string& line);
    static std::vector<std::string> readUntilEOF(std::istream& in);
};

#endif


