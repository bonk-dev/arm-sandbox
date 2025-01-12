#pragma once

namespace Logging {
	Logging::LogLevel str_to_log_level(const std::string &s);
	const char* log_level_to_str(Logging::LogLevel level);
}
