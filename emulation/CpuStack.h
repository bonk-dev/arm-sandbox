#pragma once

#include <vector>

class CpuStack {
private:
	std::vector<std::byte> _stackMemory;
	size_t _stackPointer;
public:
	explicit CpuStack(size_t stackSize);
};
