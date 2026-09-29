#ifndef TruncateCommand_h
#define TruncateCommand_h

#include "Command.h"
#include <string>

// Predstavlja komandu koja skra?uje fajl na nula bajtova
class TruncateCommand : public Command {
private:
    std::string filename;
public:
    TruncateCommand(const std::string& fname) : filename(fname) {}

    void execute(std::istream& in, std::ostream& out) override;
};

#endif
