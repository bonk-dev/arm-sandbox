#include <format>
#include "AArch64Cpu.h"

AArch64Cpu::AArch64Cpu(size_t initial_memory_size) : _memory(std::make_unique<CpuVirtualMemory>(initial_memory_size)),
													 _programCounter(0),
													 _threads() {
    this->_generalRegisters = std::vector<uint64_t>(
        AARCH64_GENERAL_PURPOSE_REGISTERS);
	this->createThread(AARCH64_MAIN_THREAD_ID);
}

void AArch64Cpu::writeGpRegister32(regindex_t index, uint32_t val) {
    this->_generalRegisters[index] = val;
}

void AArch64Cpu::writeGpRegister64(regindex_t index, uint64_t val) {
    this->_generalRegisters[index] = val;
}

uint32_t AArch64Cpu::readGpRegister32(regindex_t index) const {
    return this->_generalRegisters[index];
}

uint64_t AArch64Cpu::readGpRegister64(regindex_t index) const {
    return this->_generalRegisters[index];
}

CpuVirtualMemory &AArch64Cpu::getMemory() {
	return *this->_memory;
}

uint64_t AArch64Cpu::getProgramCounter() const {
	return this->_programCounter;
}

void AArch64Cpu::setProgramCounter(uint64_t pc) {
	this->_programCounter = pc;
}

void AArch64Cpu::createThread(uint64_t id) {
	this->getMemory().createStack(id, 8 * 1024 * 1024);
}
