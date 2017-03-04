#pragma once
#ifndef LOG_FILE
#define LOG_FILE "default.log"
#endif
#include <string>

namespace LOG
{
    //! System wide log
    bool writeLog (const char* message, ...);
    bool writeLog (const std::string message, ...);

    bool writeLogErr (const char* message, ...);
    bool writeLogErr (const std::string message, ...);
    bool restartLog();
}

