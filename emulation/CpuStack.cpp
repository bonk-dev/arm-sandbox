#include "CpuStack.h"
#include "../logging/LoggerFactory.h"
#include <numeric>
#include <iostream>

CpuStack::CpuStack(size_t stackSize):
	_logger(Logging::createLogger("CpuStack")),
	_stackMemory(std::make_unique<std::vector<std::byte>>(stackSize)),
	_stackPointer(Emulation::STACK_START) {}

void CpuStack::pop(size_t size) {
	if (this->_stackPointer + size <= std::numeric_limits<size_t>::max()) {
		this->_stackPointer += size;
	}
}

size_t CpuStack::_getVectorOffset(virtual_address_t address) const {
	const size_t vectorOffset = Emulation::STACK_START - address;
	if (vectorOffset >= this->_stackMemory->size()) {
		throw std::runtime_error("Stack overflow");
	}

	return vectorOffset;
}

void *CpuStack::_getUnsafePointer(virtual_address_t address) const {
	const size_t vecOffset = this->_getVectorOffset(address);
	return this->_stackMemory->data() + vecOffset;
}

void *CpuStack::getUnsafePointer(virtual_address_t address) const {
	_logger->verbose() << "[CpuStack] Unsafe access to " << std::showbase << std::hex << address << std::endl;
	return this->_getUnsafePointer(address);
}

std::string CpuStack::readCString(virtual_address_t address) const {
	char* ptr = reinterpret_cast<char*>(this->_getUnsafePointer(address));
	return {ptr};
}
