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

namespace NZCV {
	// Read bits

	constexpr bool Negative(const uint64_t nzcvValue) {
		return (nzcvValue >> n_offset) & 1;
	}
	constexpr bool Zero(const uint64_t nzcvValue) {
		return (nzcvValue >> z_offset) & 1;
	}
	constexpr bool Carry(const uint64_t nzcvValue) {
		return (nzcvValue >> c_offset) & 1;
	}
	constexpr bool Overflow(const uint64_t nzcvValue) {
		return (nzcvValue >> v_offset) & 1;
	}

	// Set/reset bits

	constexpr uint64_t Negative(const uint64_t nzcvValue, bool negative) {
		return set_reset_bit(nzcvValue, n_offset, negative);
	}
	constexpr uint64_t Zero(const uint64_t nzcvValue, bool zero) {
		return set_reset_bit(nzcvValue, z_offset, zero);
	}
	constexpr uint64_t Carry(const uint64_t nzcvValue, bool carry) {
		return set_reset_bit(nzcvValue, c_offset, carry);
	}
	constexpr uint64_t Overflow(const uint64_t nzcvValue, bool overflow) {
		return set_reset_bit(nzcvValue, v_offset, overflow);
	}
}