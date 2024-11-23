#include "UnconditionalBranchImmediateInstruction.h"
#include "../../numbers/two_complement.h"

InstructionDefs::UnconditionalBranchImmediateInstruction::UnconditionalBranchImmediateInstruction(uint32_t encoded) :
	is_with_link(encoded >> 31 & 1),
	immediate(UnconditionalBranchImmediateInstruction::decode_immediate(encoded)) {}

int32_t InstructionDefs::UnconditionalBranchImmediateInstruction::decode_immediate(uint32_t encoded) {
	const uint32_t raw_imm26 = (encoded & 0b11111111111111111111111111) << 2; // 26 bits
	return signed_26_bit { static_cast<int>(raw_imm26) }.val;
}
