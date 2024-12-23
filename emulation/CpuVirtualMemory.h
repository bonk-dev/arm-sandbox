#pragma once

#include <cstdio>
#include <cstdint>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <optional>
#include "emu_types.h"
#include "CpuStack.h"

constexpr uint64_t AARCH64_MAIN_THREAD_ID = 1;

/**
 * @class CpuVirtualMemory
 * @summary A simple (for now) class maintaining the virtual memory in the emulation context.
 */
class CpuVirtualMemory {
private:
	std::map<std::uint64_t, std::vector<std::byte>> _segments;

	/**
	 * @brief Key: thread id, value: thread stack
	 */
	std::map<uint64_t, std::shared_ptr<CpuStack>> _threadStacks;

	void _allocateSegmentNoOverlapCheck(virtual_address_t address, size_t size);
	std::vector<std::byte>& _getSegment(virtual_address_t virtualAddress, virtual_address_t& segmentStart);

	bool _isStackArea(const virtual_address_t address) {
		const size_t STACK_END = this->getStack(AARCH64_MAIN_THREAD_ID)->getStackSize();
		return address <= Emulation::STACK_START
			   && address >= STACK_END
		       && Emulation::STACK_START - STACK_END >= sizeof(uint32_t);
	}

public:
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
		virtual_address_t base = 0;
		auto& segment = this->_getSegment(virtualAddress, base);
		T* ptr = reinterpret_cast<T*>(segment.data() + (virtualAddress - base));
		return *ptr;
	}

	virtual_address_t allocateSegment(size_t size);
	void allocateSegment(virtual_address_t virtualAddress, size_t size);
	void freeSegment(virtual_address_t virtualAddress);

	std::shared_ptr<CpuStack> & getStack(uint64_t threadId);
	void createStack(uint64_t threadId, size_t size);
	void deleteStack(uint64_t threadId);
};
