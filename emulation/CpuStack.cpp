#include "CpuStack.h"

CpuStack::CpuStack(size_t stackSize):
	_stackMemory(stackSize),
	_stackPointer(stackSize) {}
