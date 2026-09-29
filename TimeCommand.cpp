#include "TimeCommand.h"

#include<iostream>

void TimeCommand::execute(std::istream& in, std::ostream& out) {
    std::time_t t = std::time(nullptr);
    std::tm localTime{};   
    #ifdef _WIN32
    localtime_s(&localTime, &t);
#else
    localtime_r(&t, &localTime);
#endif 

    out << std::put_time(&localTime, "%H:%M:%S")<<std::endl;
	std::ostringstream oss;
    oss << std::put_time(&localTime, "%H:%M:%S");
	setText(oss.str());
}
