#include <stdexcept>
#include "UnconditionalBranchRegister.h"

namespace {
	constexpr int opcOffset = 21;
	constexpr int op2Offset = 16;
	constexpr int op3Offset = 10;
	constexpr int regOffset = 5;

	uint32_t decode_opc(uint32_t enc) {
		return (enc >> opcOffset) & 0b1111;
	}
	uint32_t decode_op2(uint32_t enc) {
		return (enc >> op2Offset) & 0b11111;
	}
	uint32_t decode_op3(uint32_t enc) {
		return (enc >> op3Offset) & 0b111111;
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
		return ((opc == 0b0000 || opc == 0b0001 || opc == 0b0010) && op2 == 0b11111 && op3 == 0b000000 && op4 == 0b00000);
	}

	InstructionDefs::Begsi::UnconditionalBranchRegister::OperationOpc decode_opc_interp(uint32_t encoded) {
		return static_cast<InstructionDefs::Begsi::UnconditionalBranchRegister::OperationOpc>(decode_opc(encoded));
	}
}

InstructionDefs::Begsi::UnconditionalBranchRegister::UnconditionalBranchRegister(uint32_t encoded) :
	operation_opc(decode_opc_interp(encoded)),
	destination_reg((encoded >> regOffset) & 0b11111) {
	if (!is_valid(encoded)) {
		throw std::runtime_error("Unimplemented unconditional branch (register) variant");
	}
}

InstructionDefs::Begsi::UnconditionalBranchRegister::
UnconditionalBranchRegister(OperationOpc opc, regindex_t regindex) :
	operation_opc(opc),
	destination_reg(regindex) {}

uint32_t InstructionDefs::Begsi::UnconditionalBranchRegister::encode() const {
	constexpr uint32_t prefix = (0b1101011) << 25;
	const uint32_t opc = static_cast<uint32_t>(operation_opc) << opcOffset;
	constexpr uint32_t op2 = 0b11111 << op2Offset;
	constexpr uint32_t op3 = 0 << op3Offset;
	const uint32_t regIndex = destination_reg << regOffset;
	constexpr uint32_t op4 = 0;

	return prefix | opc | op2 | op3 | regIndex | op4;
}
