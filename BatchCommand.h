#ifndef BatchCommand_h
#define BatchCommand_h
#include "Command.h"
#include "Parser.h"
#include <string>
#include <vector>

//Predstavlja komandu koja izvrsava seriju drugih komandi
class BatchCommand : public Command {
    std::vector<std::string> args;
    std::string inputFile;
    std::string outputFile;
    bool append = false;

public:
    explicit BatchCommand(const ParsedCommand& p)
        : args(p.args), inputFile(p.inputFile), outputFile(p.outputFile), append(p.append) {
    }

    void execute(std::istream& in, std::ostream& out) override;
	std::vector<std::string> getArgs() { return args; }

    const std::vector<std::string>& getArgs() const { return args; }
    const std::string& getInputFile() const { return inputFile; }
    const std::string& getOutputFile() const { return outputFile; }
    bool getAppend() const { return append; }
};

#endif
