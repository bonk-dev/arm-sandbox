#pragma once
#include <memory>
#include "../AArch64Cpu.h"
#include "EmulatedSymbol.h"

namespace Emulation::Libraries {
    class SymbolNotImplemented : public EmulatedSymbol {
	private:
		std::string _symbolName;
	public:
        SymbolNotImplemented(const char* symbolName);
        void execute(AArch64Cpu& cpu) override;
    };
}
