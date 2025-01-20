#pragma once
#include "Options.h"

namespace Cli {
	enum class MenuState {
		Main = 0,
		ExecTarget = 1,
		LogLevel = 2,
		Breakpoints = 3,
		Run = 4,
		Exit = 5,
		Error = 6
	};

	class InteractiveMenu {
	private:
		MenuState _state;
		Cli::Options _options;

		std::string _error;
		void _showError(const std::string& message);

		bool _renderMain();
		bool _renderExecTarget();
		bool _renderLogLevel();
		bool _renderBreakpoints();
		bool _renderRun();
		bool _renderExit();
		bool _renderError();
	public:

		explicit InteractiveMenu(Cli::Options startingOptions);
		bool menuLoop();
		[[nodiscard]] Cli::Options getOptions() const;
		[[nodiscard]] MenuState getState() const;
	};
}
