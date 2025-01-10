#include "LoggerBase.h"
#include "NullStream.h"
#include <iostream>

std::ostream & Logging::LoggerBase::_print(std::ostream &dest, const LogLevel level) const {
    if (this->getLevel() < level) {
        return getNullStream();
    }

    dest << '[' << _prefix << "] ";
    return dest;
}

Logging::LoggerBase::LoggerBase(const char* prefix, const LogLevel level) : _level(level), _prefix(prefix) {}
Logging::LogLevel Logging::LoggerBase::getLevel() const {
    return _level;
}

std::ostream & Logging::LoggerBase::error() {
    return _print(getStream(), LogLevel::Error);
}

std::ostream & Logging::LoggerBase::info() {
    return _print(getStream(), LogLevel::Info);
}

std::ostream & Logging::LoggerBase::verbose() {
    return _print(getStream(), LogLevel::Verbose);
}


