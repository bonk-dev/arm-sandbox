#pragma once

#include <bitset>
#include <cstdint>
#include <vector>

namespace Emulation::Simd {
	static constexpr unsigned int NEON_REG_LENGTH = 128;
	static constexpr unsigned int NEON_STANDARD_REG_COUNT = 32;

	class NeonExtension {
	private:
		std::vector<std::bitset<NEON_REG_LENGTH>> _registers;

		[[nodiscard]] std::bitset<NEON_REG_LENGTH> _getVector(unsigned int index, unsigned int length) const;
	public:
		NeonExtension();
		[[nodiscard]] uint64_t readElement(unsigned int vectorIndex, unsigned int index,
										   unsigned int elementSize, unsigned int vectorLength = NEON_REG_LENGTH) const;
		void writeElement(unsigned int vectorIndex, unsigned int index,
						  unsigned int elementSize, uint64_t value, unsigned int vectorLength = NEON_REG_LENGTH);
	};
}