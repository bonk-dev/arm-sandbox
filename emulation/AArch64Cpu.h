#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "CpuVirtualMemory.h"

constexpr size_t AARCH64_GENERAL_PURPOSE_REGISTERS = 32;
typedef uint8_t regindex_t;

class AArch64Cpu {
private:
    std::vector<uint64_t> _general_registers;
	CpuVirtualMemory _memory;
public:
    explicit AArch64Cpu(size_t initial_memory_size);

    void write_gp_register_32(regindex_t index, uint32_t val);
    void write_gp_register_64(regindex_t index, uint64_t val);

    [[nodiscard]] uint32_t read_gp_register_32(regindex_t index) const;
    [[nodiscard]] uint64_t read_gp_register_64(regindex_t index) const;

	CpuVirtualMemory& get_memory();
};
