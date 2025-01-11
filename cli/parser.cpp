#include <cstring>
#include <iomanip>
#include "parser.h"

namespace {
	Logging::LogLevel parseLogLevel(const char* str, std::string& errorOut) {
		std::string s{str};
		errorOut = "";

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

		errorOut = "Invalid log level. Use --help to see available log levels.";
		return Logging::LogLevel::Invalid;
	}

	constexpr bool flagMatches(char* arg, const char* shortFlag, const char* longFlag) {
		return strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0;
	}
}

Cli::Options Cli::parseOptions(int argc, char **argv, std::string& errorOut) {
	Cli::Options opt{};

	bool parsedTarget = false;
	for (int i = 0; i < argc; ++i) {
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
			if (i++ + 1 >= argc) {
				errorOut = "You need to specify a log level after -l. Use --help to see available log levels.";
				break;
			}

			opt.logLevel = parseLogLevel(arg, errorOut);
			if (opt.logLevel == Logging::LogLevel::Invalid) {
				break;
			}
		}
	}

	if (!parsedTarget && !opt.interactive) {
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
