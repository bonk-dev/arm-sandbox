#include <cstdlib>
#include "CpuVirtualMemory.h"

// TODO: Replace with some fancy, modern C++ thingy. Probably.

CpuVirtualMemory::CpuVirtualMemory(const size_t size) {
	this->_allocatedMemory = malloc(size);
}

CpuVirtualMemory::~CpuVirtualMemory() {
	if (this->_allocatedMemory != nullptr) {
		free(this->_allocatedMemory);
	}
}
