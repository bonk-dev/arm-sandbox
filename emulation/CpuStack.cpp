#include "CpuStack.h"
#include "../logging/LoggerFactory.h"
#include <numeric>
#include <iostream>

CpuStack::CpuStack(size_t stackSize):
	_logger(Logging::createLogger("CpuStack")),
	_stackMemory(stackSize),
	_stackPointer(Emulation::STACK_START) {

	std::fill(_stackMemory.begin(), _stackMemory.end(), (std::byte)0xCC);
}

void CpuStack::pop(size_t size) {
	if (this->_stackPointer + size <= std::numeric_limits<size_t>::max()) {
		this->_stackPointer += size;
	}
}

size_t CpuStack::_getVectorOffset(virtual_address_t address) const {
	const size_t vectorOffset = this->_stackMemory.size() - (Emulation::STACK_START - address);
	if (vectorOffset >= this->_stackMemory.size()) {
		throw std::runtime_error("Stack overflow");
	}

	return vectorOffset;
}

void *CpuStack::_getUnsafePointer(virtual_address_t address) {
	const size_t vecOffset = this->_getVectorOffset(address);
	return reinterpret_cast<void *>(this->_stackMemory.data() + vecOffset);
}

void *CpuStack::getUnsafePointer(virtual_address_t address) {
	_logger->verbose() << "Unsafe access to " << std::showbase << std::hex << address << std::endl;
	return this->_getUnsafePointer(address);
}

std::string CpuStack::readCString(virtual_address_t address) {
	char* ptr = reinterpret_cast<char*>(this->_getUnsafePointer(address));
	return {ptr};
}

void CpuStack::push(const std::string& str) {
	this->_stackPointer -= str.size() + 1; // string + NULL

	const size_t vectorOffset = this->_getVectorOffset(this->_stackPointer);
	char* ptr = reinterpret_cast<char*>(this->_stackMemory.data() + vectorOffset);
	str.copy(ptr, str.size(), 0);
	*(ptr + str.size()) = '\x00';
}
