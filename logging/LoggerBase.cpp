#include "LoggerBase.h"

Logging::LoggerBase::LoggerBase(LogLevel level) : _level(level) {}
Logging::LogLevel Logging::LoggerBase::getLevel() const {
    return _level;
}

std::ostream & Logging::LoggerBase::error() {
    throw std::runtime_error("Not implemented");
}

std::ostream & Logging::LoggerBase::info() {
    throw std::runtime_error("Not implemented");
}

std::ostream & Logging::LoggerBase::verbose() {
    throw std::runtime_error("Not implemented");
}


