#pragma once

#include "instruction_type.h"
#include "decoder_utils.h"

namespace Decoding {
	InstructionType decode_reserved(uint32_t raw_instruction);
}