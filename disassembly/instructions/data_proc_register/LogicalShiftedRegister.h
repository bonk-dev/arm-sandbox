#pragma once
#include <cstdint>

#include "../../../emulation/emu_types.h"

namespace InstructionDefs::DataProcReg {
    struct LogicalShiftedRegister {
    public:
        enum class Operation : uint8_t {
            // Bitwise AND
            And = 0b00,
            // Bitwise OR
            Or = 0b01,
            // Bitwise XOR
            Xor = 0b10,
            // Bitwise AND with condititon flag update
            AndSetFlags = 0b11
        };

        enum class ShiftType : uint8_t {
            LogicalShiftLeft = 0b00,
            LogicalShiftRight = 0b01,
            ArythmeticShiftRight = 0b10,
            RotateRight = 0b11
        };
        /**
         * @brief If true, executor should operate on 64-bit registers (Xn)
         */
        bool is64Bit;

        /**
         * @brief What kind of bitwise operation should be done on the two register values.
         */
        Operation operation;

        /**
         * @brief What kind of optional shift should be done on the second register value.
         */
        ShiftType shiftType;

        /**
         * @brief If true, the executor should perform a bitwise negation of the value from the second GP register.
         */
        bool negate;

        regindex_t operand2Reg : 5;

        /**
         * @brief Optional shift amount
         */
        uint8_t shiftAmount : 6;

        regindex_t operand1Reg : 5;
        regindex_t destinationReg : 5;

        explicit LogicalShiftedRegister(uint32_t enc);
    };
}
