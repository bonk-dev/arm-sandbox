#include <stdexcept>
#include "LogicalImmediate.h"
#include "../init_helpers.h"

namespace {
	uint16_t decode_bitmask(const uint32_t enc) {
		const bool is64Bit = i::val(enc, 1, 31);
		const bool nBit = i::val(enc, 1, 22);

		if (!is64Bit && nBit) {
			throw std::runtime_error("The N bit cannot be one while the operation is 32 bit");
		}

		constexpr unsigned int imm_subfield_length = 6;
		const uint8_t immr = i::val(enc, imm_subfield_length, 16);
		const uint8_t imms = i::val(enc, imm_subfield_length, 10);

		// 32bit: imms:immr
		// 64bit: N:imms:immr
		uint16_t bitmask = (imms << imm_subfield_length) | immr;
		if (is64Bit) {
			bitmask |= (nBit << (imm_subfield_length * 2));
		}

		return bitmask;
	}
}

InstructionDefs::DataProcImm::LogicalImmediate::LogicalImmediate(const uint32_t encoded) :
	is64Bit(i::val(encoded, 1, 31)),
	operation(i::val_enum<InstructionDefs::LogicalOperation>(encoded, 2, 29)),
	bitmask(decode_bitmask(encoded)),
	destinationReg(i::val(encoded, i::regindex_length_bits, 0)),
	sourceReg(i::val(encoded, i::regindex_length_bits, 5)) {}
