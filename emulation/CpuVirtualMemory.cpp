#include <cstdlib>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "CpuVirtualMemory.h"

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
	// No need to start from the beginning every time we need to allocate a new page
	// so let's just store the last index
	uintptr_t i = 0;

	size_t allocated = 0;
	bool oom = false;
	while (allocated < neededSize && !oom) {
		const virtual_address_t pageAddress = virtual_page_address(address);
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

uintptr_t CpuVirtualMemory::_getRealAddress(virtual_address_t virtualAddress) {
	virtual_address_t pageAddress = virtual_page_address(virtualAddress);
	virtual_address_t inPageOffset = virtual_in_page_offset(virtualAddress);
	return this->_allocatedPages[pageAddress] + inPageOffset;
}

void CpuVirtualMemory::write(virtual_address_t destination,
							 std::vector<std::byte>::const_iterator begin,
							 std::vector<std::byte>::difference_type size) {
	this->_allocatePages(destination, size);

	for (virtual_address_t i = destination; i < virtual_page_address(destination + size) + PAGE_SIZE; i += PAGE_SIZE) {
		const uintptr_t realAddress = this->_getRealAddress(i);

		const size_t copySize = std::min(static_cast<size_t>(size), PAGE_SIZE);
		const auto beginIndex = static_cast<std::vector<std::byte>::difference_type>(i - destination);
		const auto endIndex = static_cast<std::vector<std::byte>::difference_type>(beginIndex + copySize);
		std::copy(begin + beginIndex, begin + endIndex, this->_memoryVector.begin() + realAddress);
	}
}
