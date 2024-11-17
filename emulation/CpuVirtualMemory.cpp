#include <cstdlib>
#include "CpuVirtualMemory.h"

// TODO: Replace with some fancy, modern C++ thingy. Probably.

CpuVirtualMemory::CpuVirtualMemory(const size_t size) {
	this->_allocatedMemory = malloc(size);
}

CpuVirtualMemory::~CpuVirtualMemory() {
	if (this->_allocatedMemory != nullptr) {
		free(this->_allocatedMemory);
		this->_allocatedMemory = nullptr;
	}
}

void CpuVirtualMemory::write_uint32(uintptr_t addr, uint32_t value) {
	printf("Writing (32) %u to virtual %lx\n", value, addr);
}
void CpuVirtualMemory::write_uint64(uintptr_t addr, uint64_t value) {
	printf("Writing (64) %lu to virtual %lx\n", value, addr);
}

uint32_t CpuVirtualMemory::read_uint32(uintptr_t addr) {
	printf("Reading (32) from virtual %lx\n", addr);
	return 32;
}
uint64_t CpuVirtualMemory::read_uint64(uintptr_t addr) {
	printf("Reading (64) from virtual %lx\n", addr);
	return 64;
}
