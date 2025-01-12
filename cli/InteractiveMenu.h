#pragma once
#include "Options.h"

namespace Cli {
	class InteractiveMenu {
	private:
		enum class State {
			Main = 0,
			ExecTarget = 1,
			LogLevel = 2,
			Run = 3,
			Exit = 4
		};

		State _state;
		Cli::Options _options;
		void _printMenu() const;
	public:
		explicit InteractiveMenu(Cli::Options startingOptions);
		bool menuLoop();
		Cli::Options getOptions() const;
	};
}
