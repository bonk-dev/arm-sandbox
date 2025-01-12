#pragma once
#include "Options.h"

namespace Cli {
	enum class MenuState {
		Main = 0,
		ExecTarget = 1,
		LogLevel = 2,
		Run = 3,
		Exit = 4,
		Error = 5
	};

	class InteractiveMenu {
	private:
		MenuState _state;
		Cli::Options _options;
		void _printMenu() const;

		std::string _error;
		void _showError(const std::string& message);
	public:

		explicit InteractiveMenu(Cli::Options startingOptions);
		bool menuLoop();
		[[nodiscard]] Cli::Options getOptions() const;
		[[nodiscard]] MenuState getState() const;
	};
}
