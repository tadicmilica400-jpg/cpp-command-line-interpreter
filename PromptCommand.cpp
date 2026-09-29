#include "PromptCommand.h"
#include "Interpreter.h"
#include "Error.h"
#include <iostream>

// Postavljamo novu vrednost prompta
void PromptCommand::execute(std::istream& in, std::ostream& out) {
    Error::checkPrompt(newPrompt);
    if (newPrompt.back() != ' ') {
        newPrompt += ' ';
    }
    Interpreter::getInstance().setPrompt(newPrompt);
}