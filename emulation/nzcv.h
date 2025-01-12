#pragma once

#include <cstdint>

namespace {
	constexpr unsigned int n_offset = 31;
	constexpr unsigned int z_offset = 30;
	constexpr unsigned int c_offset = 29;
	constexpr unsigned int v_offset = 28;

	constexpr uint64_t set_reset_bit(uint64_t val, unsigned int bit, bool flag) {
		if (flag) {
			val |= (1 << bit);
		}
		else {
			val &= ~(1 << bit);
		}
		return val;
	}
}

namespace nzcv {
	// Read bits

	constexpr bool negative(const uint64_t nzcvValue) {
		return (nzcvValue >> n_offset) & 1;
	}
	constexpr bool zero(const uint64_t nzcvValue) {
		return (nzcvValue >> z_offset) & 1;
	}
	constexpr bool carry(const uint64_t nzcvValue) {
		return (nzcvValue >> c_offset) & 1;
	}
	constexpr bool overflow(const uint64_t nzcvValue) {
		return (nzcvValue >> v_offset) & 1;
	}

	// Set/reset bits

	constexpr uint64_t negative(const uint64_t nzcvValue, bool negative) {
		return set_reset_bit(nzcvValue, n_offset, negative);
	}
	constexpr uint64_t zero(const uint64_t nzcvValue, bool zero) {
		return set_reset_bit(nzcvValue, z_offset, zero);
	}
	constexpr uint64_t carry(const uint64_t nzcvValue, bool carry) {
		return set_reset_bit(nzcvValue, c_offset, carry);
	}
	constexpr uint64_t overflow(const uint64_t nzcvValue, bool overflow) {
		return set_reset_bit(nzcvValue, v_offset, overflow);
	}
}