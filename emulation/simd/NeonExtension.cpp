#include <stdexcept>
#include <limits>
#include "NeonExtension.h"
#include "../../utils/bit_utils.h"

namespace {
	constexpr uint64_t MAX_U64 = std::numeric_limits<uint64_t>::max();

	constexpr std::bitset<128> MAX_U128_BS = ~std::bitset<128>(0);
	constexpr std::bitset<128> MAX_U64_BS = std::bitset<128>(MAX_U64);
	constexpr std::bitset<128> MAX_U32_BS = std::bitset<128>(std::numeric_limits<uint32_t>::max());
	constexpr std::bitset<128> MAX_U16_BS = std::bitset<128>(std::numeric_limits<uint16_t>::max());
	constexpr std::bitset<128> MAX_U8_BS = std::bitset<128>(std::numeric_limits<uint8_t>::max());
	constexpr std::bitset<128> MAX_BS_LUT[5] = {
		MAX_U8_BS,
		MAX_U16_BS,
		MAX_U32_BS,
		MAX_U64_BS,
		MAX_U128_BS
	};

	uint64_t bitset_substring(const std::bitset<Emulation::Simd::NEON_REG_LENGTH>& bitset,
							  const unsigned int msbIndex, const unsigned int lsbIndex) {
		if (msbIndex >= Emulation::Simd::NEON_REG_LENGTH) {
			throw std::runtime_error("MSB index must be smaller than NEON_REG_LENGTH");
		}
		if (lsbIndex < msbIndex) {
			throw std::runtime_error("LSB index must be smaller than MSB index");
		}

		const unsigned int length = msbIndex - lsbIndex + 1;
		if (length > sizeof(uint64_t)) {
			throw std::runtime_error("Substring length must be smaller than uint64_t length");
		}

		const uint64_t mask = Utils::get_mask(length);
		return (bitset >> lsbIndex).to_ullong() & mask;
	}

	const std::bitset<Emulation::Simd::NEON_REG_LENGTH>& get_bitset_mask(const unsigned int length) {
		if (length != 128 && length != 64 && length != 32 && length != 16 && length != 8) {
			throw std::runtime_error("Length must be a multiple of 2, starting from 8, ending at 128");
		}

		const unsigned int lut_index = std::countr_zero(length) - 3;
		return MAX_BS_LUT[lut_index];
	}

	void assert_vector_index(unsigned int vectorIndex) {
		if (vectorIndex >= Emulation::Simd::NEON_STANDARD_REG_COUNT) {
			throw std::runtime_error("Vector index must be smaller than NEON_STANDARD_REG_COUNT");
		}
	}
}

std::bitset<Emulation::Simd::NEON_REG_LENGTH> Emulation::Simd::NeonExtension::_getVector(unsigned int index, unsigned int length) const {
	return _registers.at(index) & get_bitset_mask(length);
}

Emulation::Simd::NeonExtension::NeonExtension() : _registers(NEON_STANDARD_REG_COUNT) {}

uint64_t
Emulation::Simd::NeonExtension::readElement(unsigned int vectorIndex, unsigned int index, unsigned int elementSize,
											unsigned int vectorLength) const {
	assert_vector_index(vectorIndex);

	const auto vec = _getVector(vectorIndex, vectorLength);
	const unsigned int msbIndex = (index + 1) * elementSize - 1;
	const unsigned int lsbIndex = index * elementSize;

	return bitset_substring(vec, msbIndex, lsbIndex);
}

void
Emulation::Simd::NeonExtension::writeElement(unsigned int vectorIndex, unsigned int index, unsigned int elementSize,
											 uint64_t value, unsigned int vectorLength) {
	assert_vector_index(vectorIndex);

	auto& vec = _registers.at(vectorIndex);
	const unsigned int lsbIndex = index * elementSize;
	const unsigned int msbIndex = (index + 1) * elementSize - 1;

	// ANDs and ORs with bitsets are done with loops anyway
	for (unsigned int i = lsbIndex; i <= msbIndex; ++i) {
		vec[i] = (value & 1);
		value >>= 1;
	}
}
