#pragma once

#include <vector>
#include <stdexcept>
#include <memory>
#include "emu_types.h"

namespace Emulation {
	constexpr size_t STACK_START = 0x7fffffffff;
}

class CpuStack {
private:
	std::unique_ptr<std::vector<std::byte>> _stackMemory;
	size_t _stackPointer;

	[[nodiscard]] size_t _getVectorOffset(virtual_address_t address) ;
public:
	explicit CpuStack(size_t stackSize);

	template <typename T>
	void push(T value) {
		this->_stackPointer -= sizeof(T);

		const size_t vectorOffset = this->_getVectorOffset(this->_stackPointer);
		T* ptr = reinterpret_cast<T*>(this->_stackMemory->data() + vectorOffset);
		*ptr = value;
	}

	template <typename T>
	void pop() {
		this->pop(sizeof(T));
	}
	void pop(size_t size);

	template <typename T>
	T read(virtual_address_t address) {
		const size_t vecOffset = this->_getVectorOffset(address);
		return *reinterpret_cast<T*>(this->_stackMemory->data() + vecOffset);
	}
};
