#pragma once

#include "../../emulation/emu_types.h"

enum class MoveWideImmediateInstructionOpType {
	Unallocated = 0b01,
	Invert = 0b00,
	Zero = 0b10,
	KeepBits = 0b11
};

typedef struct MoveWideImmediateInstruction {
	bool is_64bit;
	MoveWideImmediateInstructionOpType op_type;
	uint8_t left_shift;
	uint16_t immediate;
	regindex_t destination_reg;

	explicit MoveWideImmediateInstruction(uint32_t encoded);
} MoveWideImmediateInstruction;