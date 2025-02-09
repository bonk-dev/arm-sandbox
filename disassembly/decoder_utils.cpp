#include "decoder_utils.h"

InstructionType
Decoding::find_instruction_type_fast(std::map<mask_values_t, InstructionType> &mask_type_map, uint32_t val) {
	for (auto &[mask, type]: mask_type_map) {
		if (MATCHES_MASK(val, mask)) {
			return type;
		}
	}

	return InstructionType::Undefined;
}
