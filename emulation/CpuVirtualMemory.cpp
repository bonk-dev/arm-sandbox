#include <cstdlib>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "CpuVirtualMemory.h"

namespace {
	constexpr size_t PAGE_SIZE_2_POWER = 11;
	constexpr size_t PAGE_SIZE = (2 << PAGE_SIZE_2_POWER);
}

CpuVirtualMemory::CpuVirtualMemory(const size_t size) : _memoryVector(size) {
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

void CpuVirtualMemory::_allocatePages(virtual_address_t address, size_t neededSize) {
	const virtual_address_t pageRelativeMask = ~((2 << PAGE_SIZE_2_POWER) - 1);

	// No need to start from the beginning every time we need to allocate a new page
	// so let's just store the last index
	uintptr_t i = 0;

	size_t allocated = 0;
	bool oom = false;
	while (allocated < neededSize && !oom) {
		const virtual_address_t pageAddress = address & pageRelativeMask;
		if (this->_allocatedPages.count(pageAddress)) {
			// page already allocated
			allocated += PAGE_SIZE;
			address += PAGE_SIZE;
		}
		else {
			oom = true;

			// allocate page
			// find empty space in _memoryVector
			for (; i < this->_memoryVector.size(); i += PAGE_SIZE) {
				if (!this->_allocatedAddresses.count(i) || !this->_allocatedAddresses[i]) {
					oom = false;

					// this _memoryVector section is free
					this->_allocatedAddresses[i] = true;
					this->_allocatedPages[pageAddress] = i;
					allocated += PAGE_SIZE;
					address += PAGE_SIZE;

					std::cout << "[CpuVirtualMemory] Allocated page at " << std::hex << std::showbase << pageAddress << std::endl;

					break;
				}
			}
		}
	}

	if (allocated < neededSize) {
		throw std::runtime_error("Out-of-memory");
	}
}

void CpuVirtualMemory::write(virtual_address_t destination, const std::vector<std::byte> &data) {
	this->_allocatePages(destination, data.size());
//	this->all
}
