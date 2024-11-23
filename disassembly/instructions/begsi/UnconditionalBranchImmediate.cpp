#include "UnconditionalBranchImmediate.h"
#include "../../../numbers/two_complement.h"

InstructionDefs::Begsi::UnconditionalBranchImmediate::UnconditionalBranchImmediate(uint32_t encoded) :
	is_with_link(encoded >> 31 & 1),
	immediate(UnconditionalBranchImmediate::decode_immediate(encoded)) {}

int32_t InstructionDefs::Begsi::UnconditionalBranchImmediate::decode_immediate(uint32_t encoded) {
	const uint32_t raw_imm26 = (encoded & 0b11111111111111111111111111) << 2; // 26 bits
	return signed_26_bit { static_cast<int>(raw_imm26) }.val;
}
