#include "DateCommand.h"

#include<iostream>

// Ispisuje trenutni datum u formatu YYYY-MM-DD
void DateCommand::execute(std::istream& in, std::ostream& out) {
    std::time_t t = std::time(nullptr);
    std::tm localTime{};
    #ifdef _WIN32
    localtime_s(&localTime, &t);
#else
    localtime_r(&t, &localTime);
#endif
    out << std::put_time(&localTime, "%Y-%m-%d")<<std::endl;
    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d");
    setText(oss.str());
}
