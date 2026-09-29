#ifndef Command_h
#define Command_h

#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

// Apstraktna klasa koja predstavlja komandu
class Command
{
	std::string inputFile="";
	std::string outputFile="";
	bool append=false;
	std::string text;
public:
	void setInputFile(const std::string& file) { inputFile = file; }
	void setOutputFile(const std::string& file) { outputFile = file; }
	void setText(const std::string& t) { text = t; }
	void SetAppend(const bool ap) { append = ap; }

	const std::string& getInputFile() const { return inputFile; }
	const std::string& getOutputFile() const { return outputFile; }
	const std::string& getText() const { return text; }
	const bool getAppend() { return append; }

	~Command() = default;

	virtual void execute(std::istream& in, std::ostream& out) = 0;
};

#endif
