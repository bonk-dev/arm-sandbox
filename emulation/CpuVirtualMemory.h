#pragma once

#include <cstdio>
#include <cstdint>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <optional>
#include "emu_types.h"

namespace {
	constexpr size_t PAGE_SIZE_2_POWER = 11;
	constexpr size_t PAGE_SIZE = (2 << PAGE_SIZE_2_POWER);
	constexpr size_t virtual_page_address(virtual_address_t virtualAddress) {
		const virtual_address_t pageRelativeMask = ~((2 << PAGE_SIZE_2_POWER) - 1);
		return virtualAddress & pageRelativeMask;
	}
	constexpr size_t virtual_in_page_offset(virtual_address_t virtualAddress) {
		const virtual_address_t inPageOffsetMask = ((2 << PAGE_SIZE_2_POWER) - 1);
		return virtualAddress & inPageOffsetMask;
	}
}

/**
 * @class CpuVirtualMemory
 * @summary A simple (for now) class maintaining the virtual memory in the emulation context.
 */
class CpuVirtualMemory {
private:
	void* _allocatedMemory;
	std::vector<std::byte> _memoryVector;

	/**
	 * @brief Mapping of virtual addresses accessible by the CPU to beginning indexes of pages (_memoryVector)
	 *
	 * Every page is 4KB
	 */
	std::map<virtual_address_t, uintptr_t> _allocatedPages;

	/**
	 * @brief Key: 4KB page address, Value: true if allocated
	 */
	std::map<uintptr_t, bool> _allocatedAddresses;

	uintptr_t _getRealAddress(virtual_address_t virtualAddress);
	void _allocatePages(virtual_address_t address, size_t neededSize);
public:
	explicit CpuVirtualMemory(size_t size);
	~CpuVirtualMemory();

	void write(uintptr_t addr, int value) {
		this->write(addr, static_cast<uint32_t>(value));
	}
	void write(uintptr_t addr, long value) {
		this->write(addr, static_cast<uint64_t>(value));
	}

	void write(uintptr_t addr, uint32_t value);
	void write(uintptr_t addr, uint64_t value);

	void write(virtual_address_t destination,
			   std::vector<std::byte>::const_iterator begin,
			   std::vector<std::byte>::difference_type size);

	template<typename T>
	T read(virtual_address_t virtualAddress) {
		// TODO: Implement safe cross page read
		const uintptr_t realAddr = this->_getRealAddress(virtualAddress);
		T* ptr = reinterpret_cast<T*>(this->_memoryVector.data() + realAddr);
		return *ptr;
	}

	[[nodiscard]] uint32_t read_uint32(uintptr_t addr);
	[[nodiscard]] uint64_t read_uint64(uintptr_t addr);

	void manualAllocatePage(virtual_address_t address, size_t requiredSize) {
		this->_allocatePages(address, requiredSize);
	}
};
