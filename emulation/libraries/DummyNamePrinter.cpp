#include "DummyNamePrinter.h"

namespace Emulation::Libraries {
	void DummyNamePrinter::execute() {
		std::cout << "[DummyNamePrinter] Symbol called: " << this->_symbolName << std::endl;
	}

	DummyNamePrinter::DummyNamePrinter(const char *symbolName, [[maybe_unused]] const std::shared_ptr<AArch64Cpu> &cpu) :
		_symbolName(symbolName) {}
}