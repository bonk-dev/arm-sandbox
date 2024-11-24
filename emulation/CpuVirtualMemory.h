#pragma once

#include <cstdio>
#include <cstdint>
#include <sstream>
#include <iostream>

/**
 * @class CpuVirtualMemory
 * @summary A simple (for now) class maintaining the virtual memory in the emulation context.
 */
class CpuVirtualMemory {
private:
	void* _allocatedMemory;
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

	template<class T>
	T read(uintptr_t addr) {
		std::stringstream ss;
		ss << "[Memory] ";
		ss << sizeof(T) * 8;
		ss << "bit Read from address: " << std::hex << std::showbase << addr;

		// std::cout will be replaced
		std::cout << ss.str().c_str() << std::endl;

		return 0;
	}

	[[nodiscard]] uint32_t read_uint32(uintptr_t addr);
	[[nodiscard]] uint64_t read_uint64(uintptr_t addr);
};
