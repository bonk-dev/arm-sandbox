#include <cstdlib>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "CpuVirtualMemory.h"

CpuVirtualMemory::CpuVirtualMemory(const size_t size) :
	_memoryVector(size) {}

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
	const size_t STACK_END = this->getStack(AARCH64_MAIN_THREAD_ID)->getStackSize();
	if (addr <= Emulation::STACK_START && addr >= STACK_END && Emulation::STACK_START - STACK_END >= sizeof(uint32_t)) {
		ss << "[Memory] 32bit write to stack: " << std::hex << std::showbase << addr;
		this->getStack(AARCH64_MAIN_THREAD_ID)->write(addr, value);
	}
	else {
		ss << "[Memory] 32bit Write to address: " << std::hex << std::showbase << addr << " (val: " << value << ")";

		// std::cout will be replaced
		std::cout << ss.str().c_str() << std::endl;
	}
}

void CpuVirtualMemory::write(uintptr_t addr, uint64_t value) {
	std::stringstream ss;
	const size_t STACK_END = this->getStack(AARCH64_MAIN_THREAD_ID)->getStackSize();
	if (addr <= Emulation::STACK_START && addr >= STACK_END && Emulation::STACK_START - STACK_END >= sizeof(uint64_t)) {
		ss << "[Memory] 32bit write to stack: " << std::hex << std::showbase << addr;
		this->getStack(AARCH64_MAIN_THREAD_ID)->write(addr, value);
	}
	else {
		ss << "[Memory] 64bit Write to address: " << std::hex << std::showbase << addr << " (val: " << value << ")";

		// std::cout will be replaced
		std::cout << ss.str().c_str() << std::endl;
	}
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

std::shared_ptr<CpuStack> &CpuVirtualMemory::getStack(uint64_t threadId) {
	if (threadId != AARCH64_MAIN_THREAD_ID) {
		throw std::runtime_error("Only the main thread is supported");
	}

	// TODO: Only one thread is supported, so the default thread is always the main thread
	return this->_threadStacks.at(AARCH64_MAIN_THREAD_ID);
}

void CpuVirtualMemory::createStack(uint64_t threadId, const size_t size) {
	if (threadId != AARCH64_MAIN_THREAD_ID) {
		throw std::runtime_error("Only the main thread is supported");
	}

	this->_threadStacks.emplace(threadId, std::make_shared<CpuStack>(size));
}

void CpuVirtualMemory::deleteStack(uint64_t threadId) {
	this->_threadStacks.erase(threadId);
}

namespace {
	template<typename T>
	constexpr bool is_in_range(T start, T end, T value) {
		return value >= start && value <= end;
	}

	bool does_segment_overlap(const std::pair<virtual_address_t, std::vector<std::byte>> &pair, virtual_address_t start,
							  size_t size) {
		// If a segment starts at 0xF, and has a size of 2 bytes:
		// Start == 0xF, End = 0x10 (0xF + 2 - 1)

		// if either the requested segment starts in the already allocated region,
		// or the requested segment ends in the region
		// then it overlaps
		const virtual_address_t allocatedStart = pair.first;
		const size_t allocatedSize = pair.second.size();
		const virtual_address_t allocatedEnd = allocatedStart + allocatedSize - 1;
		bool resutl = is_in_range(allocatedStart, allocatedEnd, start)
					  || is_in_range(allocatedStart, allocatedEnd, start + size - 1);;
		return resutl;
	}
}

void CpuVirtualMemory::allocateSegment(size_t size) {
	virtual_address_t currentAddress = 0;

	// find next available virtual address;
	if (!this->_segments.empty()) {
		bool freeFound = false;
		while (!freeFound) {
			const size_t requestedEnd = currentAddress + size - 1;
			auto overlap = std::find_if(
					this->_segments.begin(), this->_segments.end(),
					[&](const auto& val)
					{
						return does_segment_overlap(val, currentAddress, size);
					});

			if (overlap == this->_segments.end()) {
				freeFound = true;
			}
			else {
				currentAddress = overlap->first + overlap->second.size();
			}
		}
	}

	this->_allocateSegmentNoOverlapCheck(currentAddress, size);
}

void CpuVirtualMemory::allocateSegment(virtual_address_t virtualAddress, size_t size) {
	const size_t requestedEnd = virtualAddress + size - 1;
	auto overlap = std::find_if(
			this->_segments.begin(), this->_segments.end(),
			[&](const auto& val)
			{
				return does_segment_overlap(val, virtualAddress, size);
			});
	if (overlap != this->_segments.end()) {
		throw std::runtime_error("Segment overlap");
	}

	this->_allocateSegmentNoOverlapCheck(virtualAddress, size);
}

void CpuVirtualMemory::_allocateSegmentNoOverlapCheck(virtual_address_t address, size_t size) {
	this->_segments.emplace(address, size);
}

void CpuVirtualMemory::freeSegment(virtual_address_t virtualAddress) {
	this->_segments.erase(virtualAddress);
}
