#include <stdexcept>
#include "UnconditionalBranchRegister.h"

namespace {
	uint32_t decode_opc(uint32_t enc) {
		return (enc >> 21) & 0b1111;
	}
	uint32_t decode_op2(uint32_t enc) {
		return (enc >> 16) & 0b11111;
	}
	uint32_t decode_op3(uint32_t enc) {
		return (enc >> 10) & 0b111111;
	}
	uint32_t decode_op4(uint32_t enc) {
		return enc & 0b11111;
	}

	bool is_valid(uint32_t encoded) {
		const uint32_t opc = decode_opc(encoded);
		const uint32_t op2 = decode_op2(encoded);
		const uint32_t op3 = decode_op3(encoded);
		const uint32_t op4 = decode_op4(encoded);

		// a lot of UnconditionalBranchRegister instructions are either marked as UNALLOCATED
		// or FEAT_PAuth (which we are not implementing for now)
		return ((opc == 0b0000 || opc == 0b0001) && op2 == 0b11111 && op3 == 0b000000 && op4 == 0b00000);
	}

	bool decode_is_link_branch(uint32_t encoded) {
		return decode_opc(encoded) == 0b0001;
	}
}

InstructionDefs::Begsi::UnconditionalBranchRegister::UnconditionalBranchRegister(uint32_t encoded) :
	branch_with_link(decode_is_link_branch(encoded)),
	destination_reg((encoded >> 5) & 0b11111) {
	if (!is_valid(encoded)) {
		throw std::runtime_error("Unimplemented unconditional branch (register) variant");
	}
}
