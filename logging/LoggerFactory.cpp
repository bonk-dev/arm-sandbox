#include "LoggerFactory.h"
#include "LoggerContainer.h"

std::unique_ptr<Logging::LoggerBase> Logging::createLogger(const char *prefix) {
	auto logger = std::make_unique<StdoutLogger>(prefix, Logging::getGlobalLogLevel());
	return logger;
}
