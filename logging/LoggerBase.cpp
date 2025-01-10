#include "LoggerBase.h"
#include "NullStream.h"
#include <iostream>
#include "LoggerContainer.h"

std::ostream & Logging::LoggerBase::_print(std::ostream &dest, const LogLevel level, bool printPrefix) const {
    if (this->getLevel() < level) {
        return getNullStream();
    }

	if (printPrefix) {
		dest << '[' << _prefix << "] ";
	}
    return dest;
}

Logging::LoggerBase::LoggerBase(const char* prefix, const LogLevel level) : _level(level), _prefix(prefix) {
	Logging::registerLogger(this);
}

Logging::LogLevel Logging::LoggerBase::getLevel() const {
    return _level;
}
void Logging::LoggerBase::setLevel(Logging::LogLevel level) {
	_level = level;
}

std::ostream & Logging::LoggerBase::error(bool printPrefix) {
    return _print(getStream(), LogLevel::Error, printPrefix);
}

std::ostream & Logging::LoggerBase::info(bool printPrefix) {
    return _print(getStream(), LogLevel::Info, printPrefix);
}

std::ostream & Logging::LoggerBase::verbose(bool printPrefix) {
    return _print(getStream(), LogLevel::Verbose, printPrefix);
}

std::ostream & Logging::LoggerBase::error() {
	return error(true);
}

std::ostream & Logging::LoggerBase::info() {
	return info(true);
}

std::ostream & Logging::LoggerBase::verbose() {
	return verbose(true);
}

Logging::LoggerBase::~LoggerBase() {
	Logging::unregisterLogger(this);
}


