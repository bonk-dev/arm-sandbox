#pragma once
#include <cstdint>

namespace Emulation {
    enum class ReservedCalls : uint16_t {
        LibraryCall = 0x01
    };
}
