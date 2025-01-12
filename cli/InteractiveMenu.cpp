#include <iostream>
#include "InteractiveMenu.h"
#include "clear-console.h"

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

	void print_header() {
		std::cout << "arm-sandbox 0.1" << std::endl;
	}

	template<typename ReturnValue>
	ReturnValue read_until_valid(const std::string& prompt, bool (*predicate)(const ReturnValue& readValue)) {
		bool invalid;
		ReturnValue c;
		do {
			invalid = false;
			std::cout << prompt;
			std::cin >> c;
			if (std::cin.bad()) {
				invalid = true;
			}
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			if (invalid) {
				continue;
			}

			invalid = !predicate(c);
		} while (invalid);

		return c;
	}
}

namespace Cli {
	InteractiveMenu::InteractiveMenu(Cli::Options startingOptions) : \
		_options(std::move(startingOptions)),
		_screen(State::Main) {}

	bool InteractiveMenu::menuLoop() {
		clear_terminal();

		switch (_screen) {
			case State::Main: {
				_printMenu();

				int screenInt = read_until_valid<int>("Choose (1-4): ", [](const int& s) {
					return s >= 1 && s <= 4;
				});
				_screen = static_cast<State>(screenInt);

				break;
			}
			case State::ExecTarget:
				print_header();
				break;
			case State::LogLevel:
				break;
			case State::Run:
				break;
			case State::Exit:
				break;
		}

		return true;
	}

	void InteractiveMenu::_printMenu() const {
		const char* emulTarget = this->_options.emulationTarget.empty()
			? "<none>"
			: this->_options.emulationTarget.c_str();

		print_header();
		std::cout << "1: Set execution target (current: " << emulTarget << ")" << std::endl;
		std::cout << "2: Set global level (current: " << logLevelToStr(_options.logLevel) << ")" << std::endl;
		std::cout << "3: Run" << std::endl;
		std::cout << "4: Exit" << std::endl;
	}

	Cli::Options InteractiveMenu::getOptions() const {
		return _options;
	}
}