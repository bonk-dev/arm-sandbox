#include "load_and_store.h"

namespace {
	// Top-level -> Load and store -> (op0 field) -> (op2 field)
	std::map<Decoding::mask_values_t, std::map<Decoding::mask_values_t, InstructionType>> load_and_store_op0_op2{
			{
					Decoding::mask_values_t(0b0011, 0b0010),
					{
							{
									// 0,0 just means that there no other instructions with the same op0 field
									Decoding::mask_values_t(0, 0),
									InstructionType::LoadStoreRegisterPair
							}
					}
			},
			{
					Decoding::mask_values_t(0b0011, 0b0011),
					{
							// these are the 4 LoadRegisterVariants, let's just check separately these to be safe
							{
									Decoding::mask_values_t(0b100100000000011, 0b000000000000000),
									InstructionType::LoadStoreRegister
							},
							{
									Decoding::mask_values_t(0b100100000000011, 0b000000000000001),
									InstructionType::LoadStoreRegister
							},
							{
									Decoding::mask_values_t(0b100100000000011, 0b000000000000010),
									InstructionType::LoadStoreRegister
							},
							{
									Decoding::mask_values_t(0b100100000000011, 0b000000000000011),
									InstructionType::LoadStoreRegister
							},
							{
									Decoding::mask_values_t(0b100000000000000, 0b100000000000000),
									InstructionType::LoadStoreRegisterUnsignedImm
							},
					}
			}
	};
}

InstructionType Decoding::decode_load_and_store_type(uint32_t raw_instruction) {
	const uint32_t op0 = raw_instruction >> 28 & 0b1111;
	const uint32_t op1 = raw_instruction >> 26 & 1;
	const uint32_t op2 = raw_instruction >> 10 & 0b111111111111111;

	std::map<Decoding::mask_values_t, InstructionType> *op2_map = Decoding::find_instruction_type(load_and_store_op0_op2, op0);
	if (op2_map != nullptr) {
		return Decoding::find_instruction_type_fast(*op2_map, op2);
	}

	return InstructionType::Undefined;
}