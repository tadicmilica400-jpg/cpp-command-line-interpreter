#ifndef HeadCommand_h
#define HeadCommand_h

#include "Command.h"

class HeadCommand : public Command {
    int count;
public:
    HeadCommand(int n) : count(n) {}

    void execute(std::istream& in, std::ostream& out) override;
};

#endif