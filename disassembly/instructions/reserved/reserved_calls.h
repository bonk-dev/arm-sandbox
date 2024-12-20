#pragma once
#include <cstdint>

namespace InstructionDefs::Reserved {
    enum class ReservedCalls : uint16_t {
        LibraryCall = 0x01
    };
}
