#include <stdexcept>
#include "LoadRegisterPair.h"

constexpr uint8_t REGISTER_MASK = 0b11111;

namespace {
	InstructionDefs::IndexingMode decode_indexing_mode(uint32_t encoded) {
		const uint8_t field = encoded >> 23 & 0b111;
		switch (field) {
			case 0b000:
				return InstructionDefs::IndexingMode::NonTemporalOffset;
			case 0b001:
				return InstructionDefs::IndexingMode::PostIndex;
			case 0b011:
				return InstructionDefs::IndexingMode::PreIndex;
			case 0b010:
				return InstructionDefs::IndexingMode::SignedOffset;
			default:
				throw std::runtime_error("Invalid indexing mode");
		}
	}

	typedef struct signed_7_bit {
		int val : 7;
	} signed_7_bit;
}

InstructionDefs::LoadsAndStores::LoadRegisterPair::LoadRegisterPair(uint32_t encoded) :
	is_wide(encoded >> 31 & 1),
	is_simd(encoded >> 26 & 1),
	encoding(decode_indexing_mode(encoded)),
	is_load(encoded >> 22 & 1),
	second_reg_index(encoded >> 10 & REGISTER_MASK),
	base_reg(encoded >> 5 & REGISTER_MASK),
	first_reg_index(encoded & REGISTER_MASK),
	immediate_value(decode_immediate(is_wide, encoded)) {}

int16_t InstructionDefs::LoadsAndStores::LoadRegisterPair::decode_immediate(bool is_wide, uint32_t encoded) {
	const uint8_t raw_imm7 = encoded >> 15 & 0b1111111;
	const int signed_imm7 = signed_7_bit {raw_imm7 }.val;

	return static_cast<int16_t>(is_wide
		? signed_imm7 * 8
		: signed_imm7 * 4
	);
}
