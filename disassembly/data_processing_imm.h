#pragma once

#include "instruction_type.h"
#include "decoder_utils.h"

namespace Decoding {
	InstructionType decode_data_processing_imm_type(uint32_t raw_instruction);
}