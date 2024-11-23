#include "FormPcRelAddress.h"
#include "../../numbers/two_complement.h"

int32_t InstructionDefs::FormPcRelAddress::decode_immediate(uint32_t encoded) {
	const uint32_t imm_lo = encoded >> 29 & 0b11;
	const uint32_t imm_hi = encoded >> 5 & 0b1111111111111111111; // 19 bits
	const uint32_t joined_imm = (imm_hi << 2) | imm_lo;

	return signed_21_bit { static_cast<int>(joined_imm) }.val;
}

InstructionDefs::FormPcRelAddress::FormPcRelAddress(uint32_t encoded) :
	rel_to_4kb_page(encoded >> 31 & 1),
	immediate(decode_immediate(encoded)),
	destination_reg_index(encoded & 0b11111) {}
