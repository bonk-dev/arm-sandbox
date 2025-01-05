#pragma once
#include <cstdint>

namespace InstructionDefs::Begsi {
    struct Hint {
    public:
        enum class Type : uint8_t {
            Nop = 0
        };

        Type hintType;
        explicit Hint(uint32_t encoded);
    };
}
