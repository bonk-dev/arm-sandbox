#pragma once

namespace Logging {
	/**
	 * @brief Parses an std::string into a Logging::LogLevel
	 * @param s The string to be parsed
	 * @return Parsed Logging::LogLevel
	 */
	Logging::LogLevel str_to_log_level(const std::string &s);

	/**
	 * @brief Converts a Logging::LogLevel to a c-string
	 * @param level The log level to be stringified
	 * @return The string representation of the log level
	 */
	const char* log_level_to_str(Logging::LogLevel level);
}
