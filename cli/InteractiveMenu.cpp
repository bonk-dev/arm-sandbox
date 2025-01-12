#include <iostream>
#include "InteractiveMenu.h"
#include "clear-console.h"
#include "../logging/utils.h"
#include <filesystem>
#include <functional>

namespace {
	void print_header() {
		std::cout << "arm-sandbox 0.1" << std::endl;
	}

	template<typename ReturnValue>
	ReturnValue read_until_valid(const std::string& prompt,
								 const std::function<bool(std::optional<ReturnValue>)> predicate) {
		bool invalid;
		ReturnValue c{};
		std::optional<ReturnValue> opt;
		std::stringstream iss;
		do {
			invalid = false;
			std::cout << prompt;

			std::string input;
			std::getline(std::cin, input);

			if (input.empty()) {
				opt.reset();
			}
			else {
				iss.clear();
				iss.str(input);
				iss >> c;

				if (iss.bad()) {
					invalid = true;
					continue;
				}

				opt.emplace(c);
			}

			invalid = !predicate(opt);
		} while (invalid);

		return c;
	}
}

namespace Cli {
	InteractiveMenu::InteractiveMenu(Cli::Options startingOptions) : \
		_options(std::move(startingOptions)),
		_state(MenuState::Main) {}

	bool InteractiveMenu::menuLoop() {
		clear_terminal();

		bool shouldContinue;
		switch (_state) {
			case MenuState::Main: {
				_printMenu();

				int screenInt = read_until_valid<int>("Choose (1-4): ", [](const auto& opt) {
					if (!opt.has_value()) {
						return false;
					}

					const auto& s = opt.value();
					return s >= 1 && s <= 4;
				});
				_state = static_cast<MenuState>(screenInt);

				shouldContinue = true;
				break;
			}
			case MenuState::ExecTarget: {
				print_header();

				bool setToEmpty = false;
				auto fileName = read_until_valid<std::string>(
						"Choose your execution target (must be an ELF64 binary, built for AArch64), or press ENTER to unset: ",
						[&setToEmpty](const auto& opt) {
							if (!opt.has_value()) {
								setToEmpty = true;
								return true;
							}

							const auto& f = opt.value();
							if (std::filesystem::is_regular_file(f)) {
								setToEmpty = false;
								return true;
							}

							std::cout << "The file does not exist" << std::endl;
							return false;
						});
				this->_options.emulationTarget = setToEmpty
						? ""
						: fileName;
				_state = MenuState::Main;

				shouldContinue = true;
				break;
			}
			case MenuState::LogLevel: {
				print_header();

				auto logLevelString = read_until_valid<std::string>(
						"Choose log level (quiet, error, warning, info, verbose): ",
						[](const auto& opt) {
							if (!opt.has_value()) {
								return false;
							}

							const auto& readLogLevel = opt.value();
							bool valid =
									readLogLevel == "quiet" ||
									readLogLevel == "error" ||
									readLogLevel == "warning" ||
									readLogLevel == "info" ||
									readLogLevel == "verbose";
							return valid;
						});

				this->_options.logLevel = Logging::str_to_log_level(logLevelString);
				_state = MenuState::Main;

				shouldContinue = true;
				break;
			}
			case MenuState::Run:
			case MenuState::Exit:
				shouldContinue = false;
				break;
		}

		return shouldContinue;
	}

	void InteractiveMenu::_printMenu() const {
		const char* emulTarget = this->_options.emulationTarget.empty()
			? "<none>"
			: this->_options.emulationTarget.c_str();

		print_header();
		std::cout << "1: Set execution target (current: " << emulTarget << ")" << std::endl;
		std::cout << "2: Set global level (current: " << Logging::log_level_to_str(_options.logLevel) << ")" << std::endl;
		std::cout << "3: Run" << std::endl;
		std::cout << "4: Exit" << std::endl;
	}

	Cli::Options InteractiveMenu::getOptions() const {
		return _options;
	}

	Cli::MenuState InteractiveMenu::getState() const {
		return _state;
	}
}