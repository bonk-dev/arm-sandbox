#include "LoadRegisterPairInstruction.h"
#include "../../numbers/two_complement.h"

constexpr uint8_t REGISTER_MASK = 0b11111;

InstructionDefs::LoadRegisterPairInstruction::LoadRegisterPairInstruction(uint32_t encoded) :
	is_wide(encoded >> 31 & 1),
	is_simd(encoded >> 26 & 1),
	encoding(static_cast<LoadStorePairEncoding>((encoded >> 23) & 0b111)),
	is_load(encoded >> 22 & 1),
	second_reg_index(encoded >> 10 & REGISTER_MASK),
	base_reg(encoded >> 5 & REGISTER_MASK),
	first_reg_index(encoded & REGISTER_MASK),
	immediate_value(decode_immediate(is_wide, encoded)) {}

int16_t InstructionDefs::LoadRegisterPairInstruction::decode_immediate(bool is_wide, uint32_t encoded) {
	const uint8_t raw_imm7 = encoded >> 15 & 0b1111111;
	const int signed_imm7 = signed_7_bit {raw_imm7 }.val;

	return static_cast<int16_t>(is_wide
		? signed_imm7 * 8
		: signed_imm7 * 4
	);
}
