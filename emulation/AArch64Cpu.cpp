#include "AArch64Cpu.h"
#include "registers.h"

AArch64Cpu::AArch64Cpu() : _memory(std::make_unique<CpuVirtualMemory>()),
													 _programCounter(0),
													 _threads(),
													 _generalRegisters(AARCH64_GENERAL_PURPOSE_REGISTERS) {
	this->createThread(AARCH64_MAIN_THREAD_ID);
}

// TODO: might rename to just writeRegister[32/64]
void AArch64Cpu::writeGpRegister32(regindex_t index, uint32_t val) {
	switch (static_cast<Emulation::Registers>(index)) {
		case Emulation::Registers::Sp:
			this->getMemory().getStack(AARCH64_MAIN_THREAD_ID)->setStackPointer(val);
			break;
		default:
			this->_generalRegisters[index] = val;
	}
}

// TODO: might rename to just writeRegister[32/64]
void AArch64Cpu::writeGpRegister64(regindex_t index, uint64_t val) {
	switch (static_cast<Emulation::Registers>(index)) {
		case Emulation::Registers::Sp:
			this->getMemory().getStack(AARCH64_MAIN_THREAD_ID)->setStackPointer(val);
			break;
		default:
			this->_generalRegisters[index] = val;
	}
}

void AArch64Cpu::writeGpRegister64(Emulation::Registers registerName, const uint64_t val) {
	writeGpRegister64(static_cast<regindex_t>(registerName), val);
}

uint32_t AArch64Cpu::readGpRegister32(regindex_t index) const {
	switch (static_cast<Emulation::Registers>(index)) {
		case Emulation::Registers::Sp:
			return this->getMemory().getStack(AARCH64_MAIN_THREAD_ID)->getStackPointer();
		default:
			return this->_generalRegisters[index];
	}
}

uint64_t AArch64Cpu::readGpRegister64(regindex_t index) const {
	switch (static_cast<Emulation::Registers>(index)) {
		case Emulation::Registers::Sp:
			return this->getMemory().getStack(AARCH64_MAIN_THREAD_ID)->getStackPointer();
		default:
			return this->_generalRegisters[index];
	}
}

CpuVirtualMemory & AArch64Cpu::getMemory() const {
	return *this->_memory;
}

uint64_t AArch64Cpu::getProgramCounter() const {
	return this->_programCounter;
}

void AArch64Cpu::setProgramCounter(uint64_t pc) {
	this->_programCounter = pc;
}

void AArch64Cpu::createThread(const uint64_t id) const {
	this->getMemory().createStack(id, 8 * 1024 * 1024);
}
