#include "CpuStack.h"

CpuStack::CpuStack(size_t stackSize):
	_stackMemory(std::make_unique<std::vector<std::byte>>(stackSize)),
	_stackPointer(stackSize) {}
