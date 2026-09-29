#ifndef WCCommand_h
#define WCCommand_h
#include "Command.h"
#include "Parser.h"
#include <string>

// Predstavlja komandu koja broji reci ili karaktere u ulaznom tekstu ili fajlu, u zavisnosti od moda koji je postavljen
class WCCommand : public Command {
public:
	// Definisanje moda brojanja: reci ili karakteri
    enum class Mode { WORDS, CHARS };

private:
    Mode mode;
    std::vector<std::string> args;
    std::unique_ptr<std::istream> inStream;

public:
	// Konstruktor koji prima mod i ParsedCommand strukturu, inicijalizujuci atribute i postavljajuci ulazni tok ako je potrebno
    WCCommand(Mode m, const ParsedCommand& pcmd)
        : mode(m) {
        args = pcmd.args;
        if (pcmd.in) {
            std::ostringstream oss;
            oss << pcmd.in->rdbuf();
            inStream = std::make_unique<std::istringstream>(oss.str());
        }
    }
    void execute(std::istream& in, std::ostream& out) override;
};
#endif