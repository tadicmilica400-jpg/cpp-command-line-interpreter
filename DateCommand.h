#ifndef DateCommand_h
#define DateCommand_h

#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>
#include "Command.h"

// Predstavlja komandu koja ispisuje trenutni datum i vreme
class DateCommand : public Command {
public:
	DateCommand() = default;
	void execute(std::istream& in, std::ostream& out) override;
};

#endif

