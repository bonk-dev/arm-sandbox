#include "CpuStack.h"

CpuStack::CpuStack(size_t stackSize):
	_stackMemory(std::make_unique<std::vector<std::byte>>(stackSize)),
	_stackPointer(stackSize) {}

void CpuStack::pop(size_t size) {
	this->_stackPointer += size;
}
