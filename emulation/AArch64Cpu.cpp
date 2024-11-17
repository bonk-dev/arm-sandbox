#include "AArch64Cpu.h"

AArch64Cpu::AArch64Cpu(size_t initial_memory_size) : _memory(initial_memory_size),
													 _program_counter(0) {
    this->_general_registers = std::vector<uint64_t>(
        AARCH64_GENERAL_PURPOSE_REGISTERS);
}

void AArch64Cpu::write_gp_register_32(const regindex_t index, const uint32_t val) {
    this->_general_registers[index] = val;
}

void AArch64Cpu::write_gp_register_64(const regindex_t index, const uint64_t val) {
    this->_general_registers[index] = val;
}

uint32_t AArch64Cpu::read_gp_register_32(const regindex_t index) const {
    return this->_general_registers[index];
}

uint64_t AArch64Cpu::read_gp_register_64(const regindex_t index) const {
    return this->_general_registers[index];
}

CpuVirtualMemory &AArch64Cpu::get_memory() {
	return this->_memory;
}

uint64_t AArch64Cpu::get_program_counter() const {
	return this->_program_counter;
}

void AArch64Cpu::set_program_counter(uint64_t pc) {
	this->_program_counter = pc;
}
