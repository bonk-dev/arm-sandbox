#pragma once

#include "LoggerBase.h"

namespace Logging {
	/**
	 * @brief Registers a logger with the internal database
	 * @details Loggers need to be registered, so that setGlobalLogLevel can change their log level
	 * @param logger The new logger
	 */
	void registerLogger(LoggerBase* logger);

	/**
	 * @brief Unregisters a logger from the internal database
	 * @details Loggers need to be unregistered, so that setGlobalLogLevel doesn't try to access an invalid logger
	 * @param logger The soon-to-be-deleted logger
	 */
	void unregisterLogger(LoggerBase* logger);

	LogLevel getGlobalLogLevel();
	/**
	 * @brief Set a new log level for all loggers (also ones created in the future)
	 * @param newLogLevel The new log level
	 */
	void setGlobalLogLevel(Logging::LogLevel newLogLevel);
}