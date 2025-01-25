#include "AArch64Cpu.h"
#include "registers.h"

namespace {
	inline void check_regindex(const regindex_t index) {
		if (index < 0 || index > 31) {
			throw std::runtime_error("Invalid register index");
		}
	}
}

AArch64Cpu::AArch64Cpu() : _nzcvConditionRegister(0),
						   _memory(std::make_unique<CpuVirtualMemory>()),
						   _programCounter(0),
						   _threads(),
						   _generalRegisters(AARCH64_GENERAL_PURPOSE_REGISTERS),
					  	   _cleanExitAddress(),
						   _halt(),
						   _exitCode(),
						   _fileSystem() {
	this->createThread(AARCH64_MAIN_THREAD_ID);
}

void AArch64Cpu::writeRegister32(regindex_t index, uint32_t val, bool useSp) {
	check_regindex(index);

	switch (static_cast<Emulation::Registers>(index)) {
		case Emulation::Registers::Sp:
			if (useSp) {
				this->getMemory().getStack(AARCH64_MAIN_THREAD_ID)->setStackPointer(val);
			}
			break;
		default:
			this->_generalRegisters[index] = val;
	}
}
void AArch64Cpu::writeRegister32(regindex_t index, uint32_t val) {
	writeRegister32(index, val, false);
}

void AArch64Cpu::writeRegister64(regindex_t index, uint64_t val, bool useSp) {
	check_regindex(index);

	switch (static_cast<Emulation::Registers>(index)) {
		case Emulation::Registers::Sp:
			if (useSp) {
				this->getMemory().getStack(AARCH64_MAIN_THREAD_ID)->setStackPointer(val);
			}
			break;
		default:
			this->_generalRegisters[index] = val;
	}
}
void AArch64Cpu::writeRegister64(regindex_t index, uint64_t val) {
	writeRegister64(index, val, false);
}

void AArch64Cpu::writeRegister64(Emulation::Registers registerName, uint64_t val, bool useSp) {
	writeRegister64(static_cast<regindex_t>(registerName), val, useSp);
}
void AArch64Cpu::writeRegister64(Emulation::Registers registerName, uint64_t val) {
	writeRegister64(registerName, val, false);
}

uint32_t AArch64Cpu::readRegister32(regindex_t index, bool useSp) const {
	check_regindex(index);

	switch (static_cast<Emulation::Registers>(index)) {
		case Emulation::Registers::Sp:
			return useSp
				? this->getMemory().getStack(AARCH64_MAIN_THREAD_ID)->getStackPointer()
				: 0;
		default:
			return this->_generalRegisters[index];
	}
}
uint32_t AArch64Cpu::readRegister32(regindex_t index) const {
	return readRegister32(index, false);
}

uint64_t AArch64Cpu::readRegister64(regindex_t index, bool useSp) const {
	check_regindex(index);

	switch (static_cast<Emulation::Registers>(index)) {
		case Emulation::Registers::Sp:
			return useSp
				? this->getMemory().getStack(AARCH64_MAIN_THREAD_ID)->getStackPointer()
				: 0;
		default:
			return this->_generalRegisters[index];
	}
}
uint64_t AArch64Cpu::readRegister64(regindex_t index) const {
	return readRegister64(index, false);
}

CpuVirtualMemory & AArch64Cpu::getMemory() const {
	return *this->_memory;
}

virtual_address_t AArch64Cpu::getCleanExitAddress() const {
	return this->_cleanExitAddress;
}

void AArch64Cpu::setCleanExitAddress(virtual_address_t address) {
	this->_cleanExitAddress = address;
}

bool AArch64Cpu::isHalted() const {
	return this->_halt;
}

int AArch64Cpu::getExitCode() const {
	return this->_exitCode;
}

void AArch64Cpu::haltExecution(const int statusCode) {
	this->_halt = true;
	this->_exitCode = statusCode;
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

Filesystem::VirtualFileSystem &AArch64Cpu::getFs() {
	return this->_fileSystem;
}

uint64_t AArch64Cpu::readNzcvRegister() const {
	return this->_nzcvConditionRegister;
}

void AArch64Cpu::writeNzcvRegsiter(const uint64_t newValue) {
	this->_nzcvConditionRegister = newValue & (0b1111 << 28);
}
