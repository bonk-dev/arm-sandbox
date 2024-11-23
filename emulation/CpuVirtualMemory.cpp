#include <cstdlib>
#include <sstream>
#include <iostream>
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

uint32_t CpuVirtualMemory::read_uint32(uintptr_t addr) {
	printf("Reading (32) from virtual %lx\n", addr);
	return 32;
}
uint64_t CpuVirtualMemory::read_uint64(uintptr_t addr) {
	printf("Reading (64) from virtual %lx\n", addr);
	return 64;
}

void CpuVirtualMemory::write(uintptr_t addr, uint32_t value) {
	std::stringstream ss;
	ss << "[Memory] 32bit Write to address: " << std::hex << std::showbase << addr << " (val: " << value << ")";

	// std::cout will be replaced
	std::cout << ss.str().c_str() << std::endl;
}

void CpuVirtualMemory::write(uintptr_t addr, uint64_t value) {
	std::stringstream ss;
	ss << "[Memory] 64bit Write to address: " << std::hex << std::showbase << addr << " (val: " << value << ")";

	// std::cout will be replaced
	std::cout << ss.str().c_str() << std::endl;
}
