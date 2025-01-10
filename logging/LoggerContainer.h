#pragma once

#include "LoggerBase.h"

namespace Logging {
	void registerLogger(LoggerBase* logger);
	void unregisterLogger(LoggerBase* logger);

	LogLevel getGlobalLogLevel();
	/**
	 * @brief Set a new log level for all loggers (also ones created in the future)
	 * @param newLogLevel The new log level
	 */
	void setGlobalLogLevel(Logging::LogLevel newLogLevel);
}