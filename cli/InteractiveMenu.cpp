#include <iostream>
#include "InteractiveMenu.h"

namespace {
	const char* logLevelToStr(Logging::LogLevel level) {
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

namespace Cli {
	InteractiveMenu::InteractiveMenu(Cli::Options startingOptions) : _options(std::move(startingOptions)) {
		_printMenu();
	}

	bool InteractiveMenu::menuLoop() {
		return true;
	}

	void InteractiveMenu::_printMenu() const {
		std::cout.clear();

		const char* emulTarget = this->_options.emulationTarget.empty()
			? "<none>"
			: this->_options.emulationTarget.c_str();

		std::cout << "arm-sandbox 0.1" << std::endl;
		std::cout << "1: Set execution target (current: " << emulTarget << ")" << std::endl;
		std::cout << "2: Set global level (current: " << logLevelToStr(_options.logLevel) << ")" << std::endl;
		std::cout << "3: Run" << std::endl;
		std::cout << "4: Exit" << std::endl;
	}

	Cli::Options InteractiveMenu::getOptions() const {
		return _options;
	}
}