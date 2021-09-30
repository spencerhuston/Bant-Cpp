#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>
#include <stdarg.h>

#define LOG(level, ...) Logger::getInstance().log(std::string(__FILE__), std::to_string(__LINE__), std::string(__FUNCTION__), level, __VA_ARGS__)
#define ERROR(errorString) Logger::error(errorString)
#define HEADER(title) Logger::getInstance().header(title)

enum Level {
    INFO,
    WARN,
    ERROR,
    FATAL,
    DEBUG
};

class Logger final {
    private:
        Logger() : logLevel(INFO) { }
        virtual ~Logger() { }

        Level logLevel;

        static constexpr int LOG_BUFFER_SIZE = 1024;
        const std::string headerString = "==============================";

    public:
        static Logger & getInstance() {
            static Logger logger;
            return logger;
        }

        void setLevel(const Level & level) noexcept { logLevel = level; }
        const Level getLevel() const noexcept { return logLevel; }

        void log(const std::string & file, const std::string & line, const std::string & function, const Level & level, const std::string fmt, ...) const {
            if (level < logLevel)
                return;

            static constexpr auto levelToString = [](const Level & level) -> std::string {
                switch (level) {
                    case INFO: return "INFO";
                    case WARN: return "WARNING";
                    case ERROR: return "ERROR";
                    case FATAL: return "FATAL";
                    case DEBUG: return "DEBUG";
                    default:
                        return "UNKNOWN";
                        break;
                }
                return "";
            };

            auto now = std::chrono::system_clock::now();
            auto msNow = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
            auto timeNow = std::chrono::system_clock::to_time_t(now);
            std::tm tmNow = *std::localtime(&timeNow);

            std::ostringstream logStringStream;
            logStringStream << std::put_time(&tmNow, "%H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << msNow.count();
            logStringStream << "  " << levelToString(level) << " => ";

            char logBuffer[LOG_BUFFER_SIZE] = {0};
            va_list args;
            va_start(args, fmt);
            vsnprintf(logBuffer, LOG_BUFFER_SIZE, fmt.c_str(), args);
            va_end(args);

            logStringStream << std::string(logBuffer) << "  [ ";
            logStringStream << file << ":" << line << ":" << function << " ]";

            std::cout << logStringStream.str() << std::endl;
        }

        static void error(const std::string & errorString) noexcept {
            std::cout << "\033[1;31m" << errorString << "\033[0m" << std::endl;
        }

        void header(const std::string & title) const noexcept {
            if (logLevel == DEBUG) {
                std::cout << headerString << std::endl 
                          << title  << std::endl 
                          << headerString << std::endl; 
            }
        }
};