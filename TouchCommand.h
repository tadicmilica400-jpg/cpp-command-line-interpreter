#ifndef TouchCommand_h
#define TouchCommand_h

#include <string>
#include <fstream>
#include "Command.h"

// Predstavlja komandu koja kreira prazan fajl cije ime dobija kao argument
class TouchCommand : public Command{

	std::string filename;

public:

	explicit TouchCommand(const std::string& file) : filename(file) {}
	void execute(std::istream& in, std::ostream& out) override;
};

#endif

