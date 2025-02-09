#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "../emulation/emu_types.h"
#include "instruction_type.h"

class A64Decoder {
private:
	uint32_t _last_raw_instruction;
public:
	InstructionType decodeNextType(uint32_t encodedInstruction);
	[[nodiscard]] uint32_t getRawInstruction() const;
};