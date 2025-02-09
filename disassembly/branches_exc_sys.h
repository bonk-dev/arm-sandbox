#pragma once

#include "instruction_type.h"
#include "decoder_utils.h"

namespace Decoding {
	InstructionType decode_branches_exc_sys(uint32_t raw_instruction);
}