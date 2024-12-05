#include "CpuStack.h"
#include <numeric>

CpuStack::CpuStack(size_t stackSize):
	_stackMemory(std::make_unique<std::vector<std::byte>>(stackSize)),
	_stackPointer(Emulation::STACK_START) {}

void CpuStack::pop(size_t size) {
	if (this->_stackPointer + size <= std::numeric_limits<size_t>::max()) {
		this->_stackPointer += size;
	}
}

size_t CpuStack::_getVectorOffset(virtual_address_t address) {
	return Emulation::STACK_START - address;
}
