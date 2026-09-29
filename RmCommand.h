#ifndef RmCommand_h
#define RmCommand_h

#include "Command.h"
#include <string>

// Predstavlja komandu koja brise fajl cije ime dobija kao argument
class RmCommand : public Command {
    std::string filename;
public:
    RmCommand(const std::string& fname) : filename(fname) {}

    void execute(std::istream& in, std::ostream& out) override;
};

#endif