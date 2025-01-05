#include "AddImmediate.h"

namespace {
	constexpr uint32_t DEST_MASK = 0b11111;
	constexpr uint32_t SOURCE_MASK = 0b11111;
	constexpr uint32_t IMM12_MASK = 0b111111111111;
}

InstructionDefs::DataProcImm::AddImmediate::AddImmediate(uint32_t encoded) :
	is_64bit(encoded >> 31 & 1),
	is_subtraction(encoded >> 30 & 1),
	set_flags(encoded >> 29 & 1),
	shift_12(encoded >> 22 & 1),
	immediate((encoded >> 10) & IMM12_MASK),
	source_reg_index((encoded >> 5) & SOURCE_MASK),
	destination_reg_index(encoded & DEST_MASK) {}
