#pragma once
#include "Options.h"

namespace Cli {
	class InteractiveMenu {
	private:
		Cli::Options _options;
		void _printMenu() const;
	public:
		InteractiveMenu();
		bool menuLoop();
	};
}
