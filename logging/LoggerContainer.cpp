#include "LoggerContainer.h"
#include <vector>

namespace {
	Logging::LogLevel _globalLogLevel = Logging::LogLevel::Verbose;
	std::vector<Logging::LoggerBase*> loggers{};
}

void Logging::registerLogger(Logging::LoggerBase *logger) {
	loggers.push_back(logger);
}

void Logging::unregisterLogger(Logging::LoggerBase *logger) {
	auto _ = std::remove(loggers.begin(), loggers.end(), logger);
	loggers.pop_back();
}

void Logging::setGlobalLogLevel(Logging::LogLevel newLogLevel) {
	_globalLogLevel = newLogLevel;
	for (auto& logger : loggers) {
		logger->setLevel(newLogLevel);
	}
}

Logging::LogLevel Logging::getGlobalLogLevel() {
	return _globalLogLevel;
}
