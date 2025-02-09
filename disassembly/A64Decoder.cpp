#include <cmath>
#include <map>
#include "A64Decoder.h"
#include "decoder_utils.h"
#include "data_processing_imm.h"
#include "scalar_fp_simd.h"

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

	InstructionType decode_branches_exc_sys(uint32_t raw_instruction) {
		const uint32_t op0 = raw_instruction >> 29 & 0b111;
		const uint32_t op1 = raw_instruction >> 12 & 0b11111111111111;

		auto op1_map = Decoding::find_instruction_type(br_exc_sys_op0_op1, op0);
		if (op1_map != nullptr) {
			return Decoding::find_instruction_type_fast(*op1_map, op1);
		}

		return InstructionType::Undefined;
	}

	// Top-level -> Data processing (register) -> (op2 field)
	std::map<Decoding::mask_values_t, InstructionType> data_proc_imm_op1zero_op2{
			{Decoding::mask_values_t(0b1000, 0b0000), InstructionType::LogicalShiftedRegister},
			{Decoding::mask_values_t(0b1001, 0b1001), InstructionType::AddSubExtendedRegister}
	};

	InstructionType decode_data_processing_register_type(uint32_t raw_instruction) {
		const uint32_t op1 = raw_instruction >> 28 & 0b1;
		const uint32_t op2 = raw_instruction >> 21 & 0b1111;

		return op1 == 0
			   ? Decoding::find_instruction_type_fast(data_proc_imm_op1zero_op2, op2)
			   : InstructionType::Undefined;
	}

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

	InstructionType decode_load_and_store_type(uint32_t raw_instruction) {
		const uint32_t op0 = raw_instruction >> 28 & 0b1111;
		const uint32_t op1 = raw_instruction >> 26 & 1;
		const uint32_t op2 = raw_instruction >> 10 & 0b111111111111111;

		std::map<Decoding::mask_values_t, InstructionType> *op2_map = Decoding::find_instruction_type(load_and_store_op0_op2, op0);
		if (op2_map != nullptr) {
			return Decoding::find_instruction_type_fast(*op2_map, op2);
		}

		return InstructionType::Undefined;
	}

	InstructionType decode_reserved(uint32_t raw_instruction) {
		const uint32_t op0 = raw_instruction >> 29 & 0b11;
		const uint32_t op1 = raw_instruction >> 16 & 0b111111111;

		// Normally this would be UDF ("permanently undefined"), but we are using it for emulating libraries
		// so kind of weird system call
		// UDF is when both op0 and op1 are both 0, but we are going to use the unallocated entry as well (op1 != 0)
		return op0 == 0
			   ? InstructionType::ReservedCall
			   : InstructionType::Undefined;
	}

	typedef InstructionType (*decode_sublevel_instruction_t)(uint32_t);

	std::map<Decoding::mask_values_t, decode_sublevel_instruction_t> top_level_op1{
			{Decoding::mask_values_t(0b1110, 0b1000), &Decoding::decode_data_processing_imm_type},
			{Decoding::mask_values_t(0b1110, 0b1010), &decode_branches_exc_sys},
			{Decoding::mask_values_t(0b0111, 0b0101), &decode_data_processing_register_type},
			{Decoding::mask_values_t(0b0111, 0b0111), &Decoding::decode_data_processing_fp_simd},
			{Decoding::mask_values_t(0b0101, 0b0100), &decode_load_and_store_type}
	};
}

InstructionType A64Decoder::decodeNextType(const uint32_t encodedInstruction) {
	this->_last_raw_instruction = encodedInstruction;

	constexpr uint8_t OP1_MASK = 0b1111;
	uint8_t op1_field = (this->_last_raw_instruction >> 25) & OP1_MASK;

	const uint32_t op0_field = encodedInstruction >> 31 & 0b1;
	if (op0_field == 0 && op1_field == 0) {
		return decode_reserved(encodedInstruction);
	}

	auto decode_func_ptr = Decoding::find_instruction_type(top_level_op1, op1_field);
	if (decode_func_ptr == nullptr) {
		return InstructionType::Undefined;
	}
	else {
		return (*decode_func_ptr)(this->_last_raw_instruction);
	}
}

uint32_t A64Decoder::getRawInstruction() const {
	return this->_last_raw_instruction;
}
