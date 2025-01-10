#include "StdoutLogger.h"
#include <iostream>
#include "NullStream.h"

Logging::StdoutLogger::StdoutLogger(const char* prefix, const LogLevel level) : LoggerBase(level), _prefix(prefix) {}

std::ostream & Logging::StdoutLogger::info() {
    if (this->getLevel() < LogLevel::Info) {
        return getNullStream();
    }

    std::cout << '[' << _prefix << "] ";
    return std::cout;
}

std::ostream & Logging::StdoutLogger::error() {
    if (this->getLevel() < LogLevel::Error) {
        return getNullStream();
    }

    std::cerr << '[' << _prefix << "] ";
    return std::cerr;
}

std::ostream & Logging::StdoutLogger::verbose() {
    if (this->getLevel() < LogLevel::Verbose) {
        return getNullStream();
    }

    std::cout << '[' << _prefix << "] ";
    return std::cout;
}