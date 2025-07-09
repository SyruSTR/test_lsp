//
// Created by grusted on 6/16/25.
//

#include "Logger.h"
#include <iostream>

Logger::Logger(const std::string& filename) {
    logFile = std::ofstream(filename);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file." << std::endl;
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(const std::string& message, LogLevel level) {
    std::string timestamp = getTimestamp();
    std::string levelStr = getLevelString(level);
    std::string logMessage = "[" + timestamp + "] [" + levelStr + "] " + message;

    // Output to file
    if (logFile.is_open()) {
        logFile << logMessage << std::endl;
    }
}

std::string Logger::getTimestamp() {
    auto now = std::time(nullptr);
    std::tm tm;
#ifdef _WIN32
    localtime_s(&tm, &now);
#else
    localtime_r(&now, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string Logger::getLevelString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::CLIENT: return "CLIENT";
        case LogLevel::SERVER: return "SERVER";
        default: return "UNKNOWN";
    }
}
