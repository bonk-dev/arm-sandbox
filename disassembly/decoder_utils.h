#pragma once

#include "instruction_type.h"
#include <tuple>
#include <cstdint>
#include <map>

namespace Decoding {
	// Explanation for the maps:
	// std::tuple<uint32_t, uint32_t> contains:
	// 	1 - mask for the field (e.g. when only the 2 MSBs are to be compared, value 1 is equal to 1100)
	//	2 - field value after ANDing the decoded value with the mask (1)
	// example:
	// ADD/SUB (immediate):
	// op1 decode field == 010x (only the first 3 most significant bits are compared)
	// (1) == 1110
	// (2) == 0100 (x are replaced with 0)
	typedef std::tuple<uint32_t, uint32_t> mask_values_t;

	constexpr bool MATCHES_MASK(uint32_t val, Decoding::mask_values_t mask_and_value) {
		const auto [mask, fixed_val] = mask_and_value;
		return (val & mask) == fixed_val;
	}

	template<class EnumType>
	EnumType *find_instruction_type(std::map<mask_values_t, EnumType> &mask_type_map, uint32_t val) {
		for (auto &[mask, type]: mask_type_map) {
			if (MATCHES_MASK(val, mask)) {
				return &type;
			}
		}

		return nullptr;
	}

	InstructionType find_instruction_type_fast(std::map<mask_values_t, InstructionType> &mask_type_map, uint32_t val);
}