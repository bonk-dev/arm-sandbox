#pragma once

#include <cstdint>

namespace Utils {
	inline uint64_t get_mask(unsigned int size) {
		// 1 << (1 - size)
		// -1
		// << 1
		// + 1
		return (((1 << (size - 1)) - 1) << 1) + 1;
	}
}