#ifndef EchoCommand_h
#define EchoCommand_h
#include "Command.h"
#include "Parser.h"

#include <string>

// Predstavlja komandu koja ispisuje argumente koje je dobila
class EchoCommand : public Command {
	std::vector<std::string> args;
public:
	EchoCommand(const std::vector<std::string>& a) : args(a) {
	}

	std::vector<std::string> GetArgs() const { return args; }	

	void execute(std::istream& in, std::ostream& out) override;
};
#endif
