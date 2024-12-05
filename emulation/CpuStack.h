#pragma once

#include <vector>
#include <stdexcept>
#include <memory>
#include "emu_types.h"

class CpuStack {
private:
	std::unique_ptr<std::vector<std::byte>> _stackMemory;
	size_t _stackPointer;
public:
	explicit CpuStack(size_t stackSize);

	template <typename T>
	void push(T value) {
		this->_stackPointer -= sizeof(T);

		const size_t vectorOffset = this->_stackMemory->size() - this->_stackPointer;
		const T* ptr = reinterpret_cast<T*>(this->_stackMemory->data() + vectorOffset);
		*ptr = value;
	}

	template <typename T>
	void pop() {
		this->pop(sizeof(T));
	}
	void pop(size_t size);
};
