#pragma once
#include <memory>
#include "../AArch64Cpu.h"
#include "EmulatedSymbol.h"
#include "../../logging/LoggerBase.h"

namespace Emulation::Libraries {
    class SymbolNotImplemented : public EmulatedSymbol {
	private:
    	std::unique_ptr<Logging::LoggerBase> _logger;
		std::string _symbolName;
	public:
        explicit SymbolNotImplemented(const char* symbolName);
        void execute(AArch64Cpu& cpu) override;
    };
}
