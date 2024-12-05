#pragma once

#include <vector>
#include <stdexcept>
#include <memory>
#include "emu_types.h"

class CpuStack {
private:
	std::unique_ptr<std::vector<std::byte>> _stackMemory;
	size_t _stackPointer;

	[[nodiscard]] size_t _getVectorOffset(virtual_address_t address) const;
public:
	explicit CpuStack(size_t stackSize);

	template <typename T>
	void push(T value) {
		this->_stackPointer -= sizeof(T);

		const size_t vectorOffset = this->_getVectorOffset(this->_stackPointer);
		if (vectorOffset >= this->_stackMemory->size()) {
			throw std::runtime_error("Stack overflow");
		}

		const T* ptr = reinterpret_cast<T*>(this->_stackMemory->data() + vectorOffset);
		*ptr = value;
	}

	template <typename T>
	void pop() {
		this->pop(sizeof(T));
	}
	void pop(size_t size);
};
