#include "DummyNamePrinter.h"

namespace Emulation::Libraries {
	void DummyNamePrinter::execute(AArch64Cpu& cpu) {
		std::cout << "[DummyNamePrinter] Symbol called: " << this->_symbolName << std::endl;
	}

	DummyNamePrinter::DummyNamePrinter(const char *symbolName) :
		_symbolName(symbolName) {}
}