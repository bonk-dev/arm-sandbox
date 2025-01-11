#pragma once

#include "Options.h"

namespace Cli {
	Cli::Options parseOptions(int argc, char** argv, std::string& errorOut);
}