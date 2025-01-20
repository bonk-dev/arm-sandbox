#include <iostream>
#include "InteractiveMenu.h"
#include "clear_terminal.h"
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
			case MenuState::Main:
				shouldContinue = _renderMain();
				break;
			case MenuState::ExecTarget:
				shouldContinue = _renderExecTarget();
				break;
			case MenuState::LogLevel:
				shouldContinue = _renderLogLevel();
				break;
			case MenuState::Breakpoints:
				shouldContinue = _renderBreakpoints();
				break;
			case MenuState::Run:
				shouldContinue = _renderRun();
				break;
			case MenuState::Exit:
				shouldContinue = _renderExit();
				break;
			case MenuState::Error: {
				shouldContinue = _renderError();
				break;
			}
			default:
				throw std::runtime_error("Invalid menu state");
		}

		return shouldContinue;
	}

	Cli::Options InteractiveMenu::getOptions() const {
		return _options;
	}

	Cli::MenuState InteractiveMenu::getState() const {
		return _state;
	}

	void InteractiveMenu::_showError(const std::string &message) {
		_state = MenuState::Error;
		_error = message;
	}

	bool InteractiveMenu::_renderMain() {
		const char* emulTarget = this->_options.emulationTarget.empty()
								 ? "<none>"
								 : this->_options.emulationTarget.c_str();

		print_header();
		std::cout << "1: Set execution target (current: " << emulTarget << ")" << std::endl;
		std::cout << "2: Set global level (current: " << Logging::log_level_to_str(_options.logLevel) << ")" << std::endl;
		std::cout << "3: Setup breakpoints" << std::endl;
		std::cout << "4: Run" << std::endl;
		std::cout << "5: Exit" << std::endl;

		int screenInt = read_until_valid<int>("Choose (1-5): ", [](const auto& opt) {
			if (!opt.has_value()) {
				return false;
			}

			const auto& s = opt.value();
			return s >= 1 && s <= 5;
		});
		_state = static_cast<MenuState>(screenInt);

		return true;
	}

	bool InteractiveMenu::_renderExecTarget() {
		print_header();

		bool setToEmpty = false;
		auto fileName = read_until_valid<std::string>(
				"Specify a path to your execution target (must be an ELF64 binary, built for AArch64), or press ENTER to unset: ",
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

		return true;
	}

	bool InteractiveMenu::_renderLogLevel() {
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

		return true;
	}

	bool InteractiveMenu::_renderBreakpoints() {
		return true;
	}

	bool InteractiveMenu::_renderRun() {
		if (this->_options.emulationTarget.empty()) {
			_showError("You must specify an emulation target.");
			return true;
		}
		else {
			return false;
		}
	}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
	bool InteractiveMenu::_renderExit() {
		return false;
	}
#pragma clang diagnostic pop

	bool InteractiveMenu::_renderError() {
		print_header();
		std::cout << _error << std::endl;
		std::cout << "Press ENTER to go back to the main menu." << std::endl;

		std::string s;
		std::getline(std::cin, s);

		_state = MenuState::Main;
		return true;
	}
}