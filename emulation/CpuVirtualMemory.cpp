#include <sstream>
#include <iostream>
#include <algorithm>
#include "CpuVirtualMemory.h"
#include <cstring>

#include "exceptions/EmulSegFault.h"

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

		const virtual_address_t end = start + size - 1;

		bool resutl = (start <= allocatedStart && end >= allocatedEnd)
			          || is_in_range(allocatedStart, allocatedEnd, start)
					  || is_in_range(allocatedStart, allocatedEnd, end);
		return resutl;
	}
}

void CpuVirtualMemory::_allocateSegmentNoOverlapCheck(virtual_address_t address, size_t size) {
	this->_segments.emplace(address, size);
}

std::vector<std::byte> &CpuVirtualMemory::_getSegment(virtual_address_t virtualAddress, virtual_address_t& segmentStart) {
	auto segmentIterator = std::find_if(this->_segments.begin(), this->_segments.end(), [&](const auto& pair) {
		const virtual_address_t segmentEnd = pair.first + pair.second.size() - 1;
		const virtual_address_t segmentStart = pair.first;
		return is_in_range(segmentStart, segmentEnd, virtualAddress);
	});
	if (segmentIterator == this->_segments.end()) {
		throw Emulation::Exceptions::EmulSegFault();
	}

	segmentStart = segmentIterator->first;
	return segmentIterator->second;
}

void CpuVirtualMemory::write(uintptr_t addr, uint32_t value) {
	if (_isStackArea(addr)) {
		_logger->verbose() << "[Memory] 32bit write to stack: " << std::hex << std::showbase << addr << std::endl;
		this->getStack(AARCH64_MAIN_THREAD_ID)->write(addr, value);
	}
	else {
		_logger->verbose() << "[Memory] 32bit Write to address: " << std::hex << std::showbase << addr << " (val: " << value << ")" << std::endl;

		virtual_address_t start;
		std::vector<std::byte>& segment = this->_getSegment(addr, start);
		const size_t offset = addr - start;
		if (offset + sizeof(uint32_t) > segment.size()) {
			throw Emulation::Exceptions::EmulSegFault();
		}

		auto* ptr = reinterpret_cast<uint32_t*>(segment.data() + offset);
		*ptr = value;
	}
}

void CpuVirtualMemory::write(uintptr_t addr, uint64_t value) {
	if (_isStackArea(addr)) {
		_logger->verbose() << "[Memory] 64bit write to stack: " << std::hex << std::showbase << addr << std::endl;
		this->getStack(AARCH64_MAIN_THREAD_ID)->write(addr, value);
	}
	else {
		_logger->verbose() << "[Memory] 64bit Write to address: " << std::hex << std::showbase << addr << " (val: " << value << ")"<< std::endl;

		virtual_address_t start;
		std::vector<std::byte>& segment = this->_getSegment(addr, start);
		const size_t offset = addr - start;
		if (offset + sizeof(uint64_t) > segment.size()) {
			throw Emulation::Exceptions::EmulSegFault();
		}

		auto* ptr = reinterpret_cast<uint64_t*>(segment.data() + offset);
		*ptr = value;
	}
}

void CpuVirtualMemory::write(virtual_address_t destination,
							 std::vector<std::byte>::const_iterator begin,
							 std::vector<std::byte>::difference_type size) {


	virtual_address_t base = 0;
	std::vector<std::byte>& segment = this->_getSegment(destination, base);
	const auto segmentIndex = static_cast<std::vector<std::byte>::difference_type>(destination - base);

	if (segment.size() - segmentIndex < size) {
		throw Emulation::Exceptions::EmulSegFault();
	}

	std::copy(begin, begin + size, segment.begin() + segmentIndex);
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

std::string CpuVirtualMemory::readCString(virtual_address_t virtual_address) {
	if (this->_isStackArea(virtual_address)) {
		return this->getStack(AARCH64_MAIN_THREAD_ID)->readCString(virtual_address);
	}

	virtual_address_t base = 0;
	const auto& segment = this->_getSegment(virtual_address, base);
	const size_t index = virtual_address - base;

	auto ptr = reinterpret_cast<const char*>(segment.data() + index);
	if (const auto length = strnlen(ptr, segment.size()); length == segment.size()) {
		throw Emulation::Exceptions::EmulSegFault();
	}

	return {ptr};
}

virtual_address_t CpuVirtualMemory::allocateSegment(size_t size) {
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
	return currentAddress;
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

void CpuVirtualMemory::freeSegment(virtual_address_t virtualAddress) {
	this->_segments.erase(virtualAddress);
}

void *CpuVirtualMemory::getUnsafePointer(virtual_address_t virtualAddress) {
	if (_isStackArea(virtualAddress)) {
		return this->getStack(AARCH64_MAIN_THREAD_ID)->getUnsafePointer(virtualAddress);
	}
	else {
		virtual_address_t base = 0;
		auto &segment = this->_getSegment(virtualAddress, base);
		const size_t index = virtualAddress - base;
		return reinterpret_cast<void *>(segment.data() + index);
	}
}
