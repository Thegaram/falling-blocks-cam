#ifndef LOGGER_H_DEFINED
#define LOGGER_H_DEFINED

#include <iostream>
#include <mutex>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

// singleton class for logging
class Logger
{
private:
    std::mutex mtx;

    Logger() { }

    void printLog(const std::string& message)
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << message << std::endl;
    }

    template <typename... Args>
    void logMessage(const std::string& level, const Args&... args)
    {
        std::time_t t = std::time(nullptr);
        std::tm* time = std::localtime(&t);

        std::string formattedLog = stringifyParams(
            formatTime(time),
            '[' + level + "] ",
            args...
        );

        printLog(formattedLog);
    }

    std::string formatTime(const std::tm* time)
    {
        std::stringstream ss;
        ss << std::setfill('0')
           << std::setw(2) << time->tm_hour << ':'
           << std::setw(2) << time->tm_min  << ':'
           << std::setw(2) << time->tm_sec;
        return ss.str();
    }

    template <typename T>
    std::string stringifyParams(T first)
    {
        std::stringstream ss;
        ss << first;
        return ss.str();
    }

    template <typename T, typename... Args>
    std::string stringifyParams(T first, const Args&... args)
    {
        std::stringstream ss;
        ss << stringifyParams(first);
        ss << " ";
        ss << stringifyParams(args...);
        return ss.str();
    }

public:
    static Logger& getInstance()
    {
        static Logger instance;
        return instance;
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    template <typename... Args>
    void info(const Args&... args)
    {
        logMessage("INFO", args...);
    }

    template <typename... Args>
    void warn(const Args&... args)
    {
        logMessage("WARN", args...);
    }

    template <typename... Args>
    void error(const Args&... args)
    {
        logMessage("ERROR", args...);
    }

    template <typename... Args>
    void debug(const Args&... args)
    {
        logMessage("DEBUG", args...);
    }

    template <typename... Args>
    void plain(const Args&... args)
    {
        printLog(stringifyParams(args...));
    }
};

#endif // LOGGER_H_DEFINED
