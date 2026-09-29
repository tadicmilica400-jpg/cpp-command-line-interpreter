#ifndef PromptCommand_h
#define PromptCommand_h

#include "Command.h"
#include <string>

// Predstavlja komandu koja menja prompt karakter
class PromptCommand : public Command {
private:
    std::string newPrompt;
public:
    PromptCommand(const std::string& prompt) : newPrompt(prompt) {}
    void execute(std::istream& in, std::ostream& out) override;
};
#endif
