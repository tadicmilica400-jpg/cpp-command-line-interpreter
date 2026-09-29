#ifndef Interpreter_h
#define Interpreter_h

#include <string>

// Singleton klasa koja predstavlja tumača komandne linije i koja pokreće glavni loop
class Interpreter
{
	std::string prompt;
public:
	Interpreter(const std::string& initialPrompt = "$ ")
		: prompt(initialPrompt) {
	}

	Interpreter(const Interpreter&) = delete;
	Interpreter& operator=(const Interpreter&) = delete;

	static Interpreter& getInstance() {
		static Interpreter instance;
		return instance;
	}

	void run();
	void setPrompt(const std::string& newPrompt) { prompt = newPrompt; }
	const std::string& getPrompt() { return prompt; }
};

#endif

