#include "instruction_type.h"
#include <cstdint>

namespace Decoding {
	[[nodiscard]] InstructionType decode_data_processing_fp_simd(uint32_t raw_instruction);
}