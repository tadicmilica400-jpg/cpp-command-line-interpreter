#ifndef TimeCommand_h
#define TimeCommand_h

#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>
#include "Command.h"

// Predstavlja komandu koja ispisuje trenutno vreme
class TimeCommand : public Command {
public:
	TimeCommand() = default;
	void execute(std::istream& in, std::ostream& out) override;
};

#endif

