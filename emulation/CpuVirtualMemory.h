#pragma once

#include <cstdio>
#include <cstdint>

/*
 * @class CpuVirtualMemory
 * @summary A simple (for now) class maintaining the virtual memory in the emulation context.
 */
class CpuVirtualMemory {
private:
	void* _allocatedMemory;
public:
	explicit CpuVirtualMemory(size_t size);
	~CpuVirtualMemory();

	void write_uint32(uintptr_t addr, uint32_t value);
	void write_uint64(uintptr_t addr, uint64_t value);

	[[nodiscard]] uint32_t read_uint32(uintptr_t addr);
	[[nodiscard]] uint64_t read_uint64(uintptr_t addr);
};
