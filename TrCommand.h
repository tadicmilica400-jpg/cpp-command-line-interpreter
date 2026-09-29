#ifndef TrCommand_h
#define TrCommand_h

#include "Command.h"
#include "Parser.h"
#include <string>

// Predstavlja komandu koja zamenjuje karaktere ili brise u ulaznom tekstu, ili iz fajla, na osnovu onoga šta je dobila kao argumente
class TrCommand : public Command {
    std::string what;
    std::string with;
    bool hasWith;
    std::string args;
    std::unique_ptr<std::istream> inStream;
public:
	// Konstruktor koji prima ParsedCommand strukturu i inicijalizuje potrebne atribute, 
	// ukljucujuci i spajanje argumenata u jedan string i postavljanje ulaznog toka ako je potrebno
    TrCommand(const ParsedCommand& pcmd)
        : what(pcmd.what), with(pcmd.with), hasWith(!pcmd.with.empty())
    {
        args.clear();
        size_t startIndex = 0;
        if (pcmd.args.size() > startIndex) {
            args = pcmd.args[startIndex];
            for (size_t i = startIndex + 1; i < pcmd.args.size(); ++i) {
                args += " " + pcmd.args[i];
            }
        }
        if (pcmd.in) {
            std::ostringstream oss;
            oss << pcmd.in->rdbuf();
            inStream = std::make_unique<std::istringstream>(oss.str());
        }
    }

    void execute(std::istream& in, std::ostream& out) override;
};

#endif
