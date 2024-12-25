#pragma once
#include <cstdint>

namespace i {
    constexpr unsigned int val(const uint32_t encoded, const unsigned int bitLength, const unsigned int shift) {
        const uint32_t mask = (1 << bitLength) - 1;
        return (encoded >> shift) & mask;
    }

    template<typename EnumType>
    constexpr EnumType val_enum(const uint32_t encoded, const unsigned int bitLength, const unsigned int shift) {
        return static_cast<EnumType>((encoded >> shift) & (1 << bitLength) - 1);
    }
}
