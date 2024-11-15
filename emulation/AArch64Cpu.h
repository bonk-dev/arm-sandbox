#pragma once
#include <cstdint>
#include <memory>
#include <vector>

constexpr size_t AARCH64_GENERAL_PURPOSE_REGISTERS = 32;
typedef uint8_t reg_t;

class AArch64Cpu {
private:
    std::vector<uint64_t> _general_registers;
public:
    AArch64Cpu();

    void write_gp_register_32(reg_t index, uint32_t val);
    void write_gp_register_64(reg_t index, uint64_t val);

    [[nodiscard]] uint32_t read_gp_register_32(reg_t index) const;
    [[nodiscard]] uint64_t read_gp_register_64(reg_t index) const;
};
