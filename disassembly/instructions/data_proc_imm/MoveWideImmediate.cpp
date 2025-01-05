#include <stdexcept>
#include "MoveWideImmediate.h"

namespace {
	InstructionDefs::DataProcImm::MoveWideImmediateOpType decode_op_type(uint32_t encoded) {
		const uint8_t opc_field = encoded >> 29 & 0b11;
		return static_cast<InstructionDefs::DataProcImm::MoveWideImmediateOpType>(opc_field);
	}

	uint8_t decode_left_shift(uint32_t encoded) {
		uint8_t divided_by_16 = encoded >> 21 & 0b11;
		const bool is_64bit = encoded >> 31 & 1;

		if (divided_by_16 != 1 && divided_by_16 != 0 && !is_64bit) {
			throw std::runtime_error(
					"32-bit MOV (wide immediate) cannot have a left shift value different than 16 or 0");
		}

		// shifting to the left by 4 bits multiplies by 16 (2^4)
		return divided_by_16 << 4;
	}
}

InstructionDefs::DataProcImm::MoveWideImmediate::MoveWideImmediate(uint32_t encoded) :
	is_64bit(encoded >> 31 & 1),
	op_type(decode_op_type(encoded)),
	left_shift(decode_left_shift(encoded)),
	immediate(encoded >> 5 & 0b1111111111111111),
	destination_reg(encoded & 0b11111) {}
