#pragma once

#include "../../emulation/emu_types.h"

namespace InstructionDefs {
	enum class MoveWideImmediateOpType {
		Unallocated = 0b01,
		Invert = 0b00,
		Zero = 0b10,
		KeepBits = 0b11
	};

	typedef struct MoveWideImmediate {
		bool is_64bit;
		MoveWideImmediateOpType op_type;
		uint8_t left_shift;
		uint16_t immediate;
		regindex_t destination_reg;

		explicit MoveWideImmediate(uint32_t encoded);
	} MoveWideImmediateInstruction;
}