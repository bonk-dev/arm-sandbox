#pragma once
#include <string>
#include "../logging/LogLevel.h"
#include "../emulation/emu_types.h"
#include <vector>

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

		/**
		 * @brief List of virtual addresses to break on
		 */
		std::vector<virtual_address_t> breakpoints;
	};
}
