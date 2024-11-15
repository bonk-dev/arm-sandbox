#pragma once

#include <cstdio>

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
};
