#include <cstring>
#include <iomanip>
#include "parser.h"
#include "../logging/utils.h"

namespace {
	constexpr bool flagMatches(char* arg, const char* shortFlag, const char* longFlag) {
		return strcmp(arg, shortFlag) == 0 || strcmp(arg, longFlag) == 0;
	}
}

Cli::Options Cli::parseOptions(int argc, char **argv, std::string& errorOut) {
	Cli::Options opt{};

	bool parsedTarget = false;
	for (int i = 1; i < argc; ++i) {
		char* arg = argv[i];

		if (arg[0] != '-') {
			if (parsedTarget) {
				errorOut = "You can only specify one emulation target. Use --help for usage.";
				break;
			}

			opt.emulationTarget = arg;
			parsedTarget = true;
		}
		else if (flagMatches(arg, "-h", "--help")) {
			opt.showHelp = true;

			// no need to parse further options
			break;
		}
		else if (flagMatches(arg, "-i", "--interactive")) {
			opt.interactive = true;
		}
		else if (flagMatches(arg, "-l", "--log-level")) {
			if (i + 1 >= argc) {
				errorOut = "You need to specify a log level after -l. Use --help to see available log levels.";
				break;
			}

			opt.logLevel = Logging::str_to_log_level(argv[++i]);
			if (opt.logLevel == Logging::LogLevel::Invalid) {
				errorOut = "Invalid log level. Use --help to see available log levels.";
				break;
			}
		}
	}

	if (!parsedTarget && !opt.interactive && !opt.showHelp) {
		errorOut = "You must specify an emulation target. Use --help for usage.";
	}

	return opt;
}

void Cli::printUsage(Logging::LoggerBase& logger) {
	logger.error(false) << "Usage: ./arm_sandbox [options] <emulation target path>" << std::endl
		<< "-h | --help | Print this help." << std::endl
		<< "-i | --interactive | Display an interactive menu on startup" << std::endl
		<< "-l | --log-level | <quiet,error,warning,info,verbose> | Set a global log level" << std::endl
		<< std::setw(4) << ' ' << "quiet: Only display emulation target out, err and in streams." << std::endl
		<< std::setw(4) << ' ' << "error: quiet level messages and emulator errors." << std::endl
		<< std::setw(4) << ' ' << "warning: error level messages and emulator warnings." << std::endl
		<< std::setw(4) << ' ' << "info: warning level messages and emulator infos." << std::endl
		<< std::setw(4) << ' ' << "verbose: info level messages and verbose messages." << std::endl;
}
