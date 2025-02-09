#include "data_processing_register.h"

namespace {
	// Top-level -> Data processing (register) -> (op2 field)
	std::map<Decoding::mask_values_t, InstructionType> data_proc_imm_op1zero_op2{
			{Decoding::mask_values_t(0b1000, 0b0000), InstructionType::LogicalShiftedRegister},
			{Decoding::mask_values_t(0b1001, 0b1001), InstructionType::AddSubExtendedRegister}
	};
}

InstructionType Decoding::decode_data_processing_register_type(uint32_t raw_instruction) {
	const uint32_t op1 = raw_instruction >> 28 & 0b1;
	const uint32_t op2 = raw_instruction >> 21 & 0b1111;

	return op1 == 0
		   ? Decoding::find_instruction_type_fast(data_proc_imm_op1zero_op2, op2)
		   : InstructionType::Undefined;
}