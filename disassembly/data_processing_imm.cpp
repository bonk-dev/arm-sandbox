#include "data_processing_imm.h"

namespace {
	// Top-level -> Data processing (immediate) -> (op1 field)
	std::map<Decoding::mask_values_t, InstructionType> data_proc_imm_op1{
			{Decoding::mask_values_t(0b1110, 0b0100), InstructionType::AddOrSubImmediate},
			{Decoding::mask_values_t(0b1100, 0b0000), InstructionType::PcRelativeAddressing},
			{Decoding::mask_values_t(0b1110, 0b1000), InstructionType::LogicalImmediate},
			{Decoding::mask_values_t(0b1110, 0b1010), InstructionType::MoveWideImmediate}
	};
}

InstructionType Decoding::decode_data_processing_imm_type(uint32_t raw_instruction) {
	// op0 unused for now
	const uint32_t op0 = raw_instruction >> 29 & 0b11;
	const uint32_t op1 = raw_instruction >> 22 & 0b1111;

	InstructionType *ptr = Decoding::find_instruction_type(data_proc_imm_op1, op1);
	if (ptr == nullptr) {
		return InstructionType::Undefined;
	} else {
		return *ptr;
	}
}