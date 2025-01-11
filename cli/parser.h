#pragma once

#include "Options.h"
#include "../logging/LoggerBase.h"

namespace Cli {
	Cli::Options parseOptions(int argc, char** argv, std::string& errorOut);
	void printUsage(Logging::LoggerBase& logger);
}