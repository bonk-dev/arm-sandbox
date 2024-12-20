#pragma once
#include <memory>

#include "../AArch64Cpu.h"

namespace Emulation::Libraries {
    class DummyNamePrinter {
	private:
		std::string _symbolName;
	public:
        DummyNamePrinter(const char* symbolName, [[maybe_unused]] const std::shared_ptr<AArch64Cpu>& cpu);
        void execute();
    };
}
