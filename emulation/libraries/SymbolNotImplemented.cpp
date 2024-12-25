#include "SymbolNotImplemented.h"

namespace Emulation::Libraries {
	void SymbolNotImplemented::execute(AArch64Cpu& cpu) {
		std::cout << "[SymbolNotImplemented] Symbol called: " << this->_symbolName << std::endl;
	}

	SymbolNotImplemented::SymbolNotImplemented(const char *symbolName) :
		_symbolName(symbolName) {}
}