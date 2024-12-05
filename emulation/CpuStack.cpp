#include "CpuStack.h"
#include <numeric>

CpuStack::CpuStack(size_t stackSize):
	_stackMemory(std::make_unique<std::vector<std::byte>>(stackSize)),
	_stackPointer(stackSize) {}

void CpuStack::pop(size_t size) {
	if (this->_stackPointer + size <= std::numeric_limits<size_t>::max()) {
		this->_stackPointer += size;
	}
}
