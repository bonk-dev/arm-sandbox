#pragma once

#include <vector>
#include <memory>

class CpuStack {
private:
	std::unique_ptr<std::vector<std::byte>> _stackMemory;
	size_t _stackPointer;
public:
	explicit CpuStack(size_t stackSize);
};
