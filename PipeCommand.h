#ifndef PipeCommand_h
#define PipeCommand_h

#include "Command.h"
#include "Parser.h"
#include <string>

// Predstavlja komandu koja povezuje vise komandi u pipeline koristeci pipe operator |,
// sto znaci da izlaz jedne komande postaje ulaz sledece komande
class PipeCommand : public Command {
    std::vector<std::unique_ptr<Command>> commands;
    std::string inputFile;
    std::string outputFile;
    bool append; 
public:
    PipeCommand(std::vector<std::unique_ptr<Command>>&& cmds,
        const std::string& inFile,
        const std::string& outFile,
        bool app)
        : commands(std::move(cmds)), inputFile(inFile), outputFile(outFile), append(app) {
    }
    void execute(std::istream& in, std::ostream& out) override;
};

#endif

