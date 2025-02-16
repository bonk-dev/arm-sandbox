#include "SymbolNotImplemented.h"
#include <format>

namespace Emulation::Libraries {
	void SymbolNotImplemented::execute(AArch64Cpu& cpu) {
		_logger->error() << "Symbol called: " << this->_symbolName << std::endl;
		throw std::runtime_error(
			std::format("The CPU tried to call a function: \"{}\", but there was no implementation provided",
			this->_symbolName));
	}

	SymbolNotImplemented::SymbolNotImplemented(const char *symbolName) :
		EmulatedSymbol("SymbolNotImplemented"),
		_symbolName(symbolName) {}
}
