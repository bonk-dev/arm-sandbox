#pragma once
#include <string>
#include "../logging/LogLevel.h"

namespace Cli {
	/**
	 * @brief Command-line options, values
	 */
	struct Options {
		/**
		 * @brief If true, the app should start with a menu
		 */
		bool interactive;

		/**
		 * @brief The ELF binary to run
		 */
		std::string emulationTarget;

		/**
		 * @brief The log level to use for the duration of the emulation
		 */
		Logging::LogLevel logLevel;

		/**
		 * @brief If true, the app should print the usage and exit immediately.
		 */
		bool showHelp;
	};
}
