#pragma once

#include "instruction_type.h"
#include "decoder_utils.h"

namespace Decoding {
	InstructionType decode_load_and_store_type(uint32_t raw_instruction);
}