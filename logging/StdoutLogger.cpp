#include "StdoutLogger.h"
#include <iostream>
#include "NullStream.h"

std::ostream & Logging::StdoutLogger::getStream() {
    return std::cout;
}

Logging::StdoutLogger::StdoutLogger(const char* prefix, const LogLevel level) : LoggerBase(prefix, level) {}
