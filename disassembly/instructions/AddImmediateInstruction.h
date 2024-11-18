#pragma once

#include <cstdint>

typedef struct AddImmediateInstruction {
	bool is_64bit;
	bool is_subtraction;
	bool set_flags;
	bool shift_12;
	uint16_t immediate; // 0-4095, 12 bits

	uint8_t destination_reg_index;
	uint8_t source_reg_index;

	explicit AddImmediateInstruction(uint32_t encoded);
} AddImmediateInstruction;