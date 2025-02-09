#include "branches_exc_sys.h"

namespace {
	// Top-level -> Branches, Exception Generating and System instructions -> (op0 field) -> (op1 field)
	std::map<Decoding::mask_values_t, std::map<Decoding::mask_values_t, InstructionType>> br_exc_sys_op0_op1{
			{
					Decoding::mask_values_t(0b111, 0b010),
					{
							{Decoding::mask_values_t(0b11000000000000, 0), InstructionType::ConditionalBranchImmediate}
					}
			},
			{
					Decoding::mask_values_t(0b011, 0b000),
					{
							// 0,0 just means that there no other instructions with the same op0 field
							{Decoding::mask_values_t(0, 0), InstructionType::UnconditionalBranchImmediate}
					}
			},
			{
					Decoding::mask_values_t(0b011, 0b001),
					{
							{Decoding::mask_values_t(0b10000000000000, 0b00000000000000), InstructionType::CompareAndBranchImmediate}
					}
			},
			{
					Decoding::mask_values_t(0b111, 0b110),
					{
							{Decoding::mask_values_t(0b11111111111111, 0b01000000110010), InstructionType::Hint},
							{Decoding::mask_values_t(0b10000000000000, 0b10000000000000), InstructionType::UnconditionalBranchRegister}
					}
			}
	};
}

InstructionType Decoding::decode_branches_exc_sys(uint32_t raw_instruction) {
	const uint32_t op0 = raw_instruction >> 29 & 0b111;
	const uint32_t op1 = raw_instruction >> 12 & 0b11111111111111;

	auto op1_map = Decoding::find_instruction_type(br_exc_sys_op0_op1, op0);
	if (op1_map != nullptr) {
		return Decoding::find_instruction_type_fast(*op1_map, op1);
	}

	return InstructionType::Undefined;
}
