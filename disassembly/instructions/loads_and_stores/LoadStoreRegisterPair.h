#pragma once

#include <cstdint>
#include "../IndexingMode.h"

namespace InstructionDefs::LoadsAndStores {
	struct LoadStoreRegisterPair {
	private:
		static int16_t decode_immediate(bool is_wide, uint32_t encoded);

	public:
		/*
		 * For non-SIMD: if true, the instruction is 64-bit, 32-bit otherwise
		 * For SIMD: if true, the instruction is 128-bit, 64-bit otherwise
		 */
		bool is_wide;
		bool is_simd;
		InstructionDefs::IndexingMode encoding;
		bool is_load;

		uint8_t first_reg_index;
		uint8_t second_reg_index;
		uint8_t base_reg;
		int16_t immediate_value;

		explicit LoadStoreRegisterPair(uint32_t encoded);
	};
}