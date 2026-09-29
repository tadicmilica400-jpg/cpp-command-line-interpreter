#ifndef CommandBuilder_h
#define CommandBuilder_h

#include <string>
#include <memory>
#include "Command.h"
#include "Parser.h"

// Klasa koja pravi odgovarajucu komandu na osnovu ParsedCommand strukture koja joj prosledi sve potrebne informacije
class CommandBuilder {
public:
	static std::unique_ptr<Command> create(const ParsedCommand& line);
};

#endif

