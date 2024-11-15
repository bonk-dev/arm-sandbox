//
// Created by bonk on 15.11.24.
//

#include "AArch64Cpu.h"

AArch64Cpu::AArch64Cpu() {
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
