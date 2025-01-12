#include <string>
#include <stdexcept>
#include "LogLevel.h"

namespace Logging {
	Logging::LogLevel str_to_log_level(const std::string& s) {
		if (s == "verbose") {
			return Logging::LogLevel::Verbose;
		}
		if (s == "info") {
			return Logging::LogLevel::Info;
		}
		if (s == "warning") {
			return Logging::LogLevel::Warning;
		}
		if (s == "error") {
			return Logging::LogLevel::Error;
		}
		if (s == "quiet") {
			return Logging::LogLevel::Quiet;
		}

		return Logging::LogLevel::Invalid;
	}

	const char* log_level_to_str(Logging::LogLevel level) {
		switch (level) {
			case Logging::LogLevel::Quiet:
				return "quiet";
			case Logging::LogLevel::Error:
				return "error";
			case Logging::LogLevel::Warning:
				return "warning";
			case Logging::LogLevel::Info:
				return "info";
			case Logging::LogLevel::Verbose:
				return "verbose";
			default:
				throw std::runtime_error("Invalid log level");
		}
	}
}
