#pragma once
#include "Options.h"

namespace Cli {
	enum class MenuState {
		Main = 0,
		ExecTarget = 1,
		LogLevel = 2,
		Run = 3,
		Exit = 4
	};

	class InteractiveMenu {
	private:
		MenuState _state;
		Cli::Options _options;
		void _printMenu() const;
	public:

		explicit InteractiveMenu(Cli::Options startingOptions);
		bool menuLoop();
		[[nodiscard]] Cli::Options getOptions() const;
		MenuState getState() const;
	};
}
