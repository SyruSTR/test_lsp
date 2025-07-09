//
// Created by grusted on 6/16/25.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <iomanip>

enum class LogLevel {
    CLIENT,
    SERVER,
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    Logger(const std::string& filename);
    ~Logger();

    void log(const std::string& message, LogLevel level);

private:
    std::ofstream logFile;
    std::string getTimestamp();
    std::string getLevelString(LogLevel level);
};



#endif //LOGGER_H
