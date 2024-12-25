#pragma once
#include <cstdint>

namespace InstructionDefs::Reserved {
    /**
     * @brief Immediate value for Exit (0x00) reserved call: this exit is the result of the return from int main(...)
     */
    constexpr uint16_t IMM_EXIT_CLEAN_EXIT = 0;

    enum class ReservedCalls : uint16_t {
        Exit = 0x00,
        LibraryCall = 0x01
    };
}
