#ifndef Error_h
#define Error_h

#include <string>
#include <vector>
#include <set>
#include <stdexcept>
#include <fstream>
#include <iostream>

// Klasa koja sadrzi staticke metode za proveru i izbacivanje razlicitih gresaka
class Error{
public:
    Error() = default;
    ~Error() = default;

    // Leksičke greške
    static void checkLineLength(const std::string& line);
    static void unexpectedCharacters(const std::string& line);

    // Sintaksne greške
    static void checkCommandExists(const std::string& command);
    static void checkArgumentsCount(const std::string& command, const std::vector<std::string>& args, size_t expected);
    static void chechWhat(const std::string& command, const std::string& what);
	static void checkOptionsCount(const std::string& command, const std::vector<std::string>& options, size_t maxAllowed);

    //Tr format
    static void checkTrForm(const std::string& command, const std::string& what);

    // Redirekcije
    static void inputRedirectionError(const std::vector<std::string>& args, const std::string& input);
    static void outputRedirectionError(const std::vector<std::string>& args, const std::string& output);

    // OS greške
    static void fileExist(std::ifstream& file);

    // Pomocne funkcije
    static void printErrorWithPointer(const std::string& line, size_t pos, const std::string& msg);
    
	// Provera da li fajl postoji
    static void checkFileExists(const std::string& filename);
    static void checkFileExistsTouch(const std::string& filename);
    static void checkFileNotExists(const std::string& filename);

    // Provera za prompt
	static void checkPrompt(const std::string& prompt);
};

#endif
