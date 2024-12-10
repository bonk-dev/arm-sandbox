#include "A64Decoder.h"
#include "../numbers/two_complement.h"
#include <utility>
#include <cmath>
#include <stdexcept>
#include <map>

// Explanation for the maps:
// std::tuple<uint32_t, uint32_t> contains:
// 	1 - mask for the field (e.g. when only the 2 MSBs are to be compared, value 1 is equal to 1100)
//	2 - field value after ANDing the decoded value with the mask (1)
// example:
// ADD/SUB (immediate):
// op1 decode field == 010x (only the first 3 most significant bits are compared)
// (1) == 1110
// (2) == 0100 (x are replaced with 0)
typedef std::tuple<uint32_t, uint32_t> mask_values_t;

constexpr bool MATCHES_MASK(uint32_t val, mask_values_t mask_and_value) {
	const auto [mask, fixed_val] = mask_and_value;
	return (val & mask) == fixed_val;
}

template<class EnumType>
void find_instruction_type(std::map<mask_values_t, EnumType>& mask_type_map, uint32_t val, EnumType& result) {
	for (const auto& [mask, type] : mask_type_map) {
		if (MATCHES_MASK(val, mask)) {
			result = type;
			return;
		}
	}
}

uint32_t read_uint_le(const std::vector<std::byte>& v, const int index) {
	uint32_t value = 0;
	for (int i = 3; i >= 0; --i) {
		value |= static_cast<uint32_t>(v[index + i]) << (i * 8);
	}
	return value;
}

// Top-level -> Data processing -> (op1 field)
static std::map<mask_values_t, InstructionType> data_proc_op1 {
		{ mask_values_t(0b1110, 0b0100), InstructionType::AddOrSubImmediate },
		{ mask_values_t(0b1100, 0b0000), InstructionType::PcRelativeAddressing },
		{ mask_values_t(0b1110, 0b1010), InstructionType::MoveWideImmediate }
};
InstructionType decode_data_processing_type(uint32_t raw_instruction) {
	// op0 unused for now
	const uint32_t op0 = raw_instruction >> 29 & 0b11;
	const uint32_t op1 = raw_instruction >> 22 & 0b1111;

	InstructionType result = InstructionType::Undefined;
	find_instruction_type(data_proc_op1, op1, result);
	return result;
}

// Top-level -> Branches, Exception Generating and System instructions -> (op0 field)
static std::map<mask_values_t, InstructionType> br_exc_sys_op0 {
		{ mask_values_t(0b011, 0b000), InstructionType::UnconditionalBranchImmediate }
};
InstructionType decode_branches_exc_sys(uint32_t raw_instruction) {
	const uint32_t op0 = raw_instruction >> 29 & 0b111;
	const uint32_t op1 = raw_instruction >> 12 & 0b11111111111111;

	InstructionType result = InstructionType::Undefined;
	find_instruction_type(br_exc_sys_op0, op0, result);
	return result;
}

// Top-level -> Load and store -> (op0 field) -> (op2 field)
static std::map<mask_values_t , std::map<mask_values_t, InstructionType>> load_and_store_op0_op2 {
		{
			mask_values_t(0b0011, 0b0010),
		  	{
				{
				// 0,0 just means that there no other instructions with the same op0 field
				mask_values_t(0, 0),
				InstructionType::LoadStoreRegisterPair
				}
		    }
	    },
		{
			mask_values_t(0b0011, 0b0011),
			{
				{
					mask_values_t(0b100000000000000, 0b100000000000000),
					InstructionType::LoadStoreRegisterUnsignedImm
				}
			}
		}
};
InstructionType decode_load_and_store_type(uint32_t raw_instruction) {
	const uint32_t op0 = raw_instruction >> 28 & 0b1111;
	const uint32_t op1 = raw_instruction >> 26 & 1;
	const uint32_t op2 = raw_instruction >> 10 & 0b111111111111111;

	// TODO: check if a copy occurs here
	std::map<mask_values_t, InstructionType> op2_map {};
	find_instruction_type(load_and_store_op0_op2, op0, op2_map);

	InstructionType result = InstructionType::Undefined;
	if (!op2_map.empty()) {
		find_instruction_type(op2_map, op2, result);
	}

	return result;
}

typedef InstructionType (*decode_sublevel_instruction_t)(uint32_t);
static std::map<mask_values_t, decode_sublevel_instruction_t> top_level_op1 {
		{ mask_values_t(0b1110, 0b1000), &decode_data_processing_type },
		{ mask_values_t(0b1110, 0b1010), &decode_branches_exc_sys },
		{ mask_values_t(0b0101, 0b0100), &decode_load_and_store_type }
};
InstructionType A64Decoder::decodeNextType(const uint32_t encodedInstruction) {
	this->_last_raw_instruction = encodedInstruction;

	constexpr uint8_t OP1_MASK = 0b1111;
	uint8_t op1_field = (this->_last_raw_instruction >> 25) & OP1_MASK;

	decode_sublevel_instruction_t decode_func = nullptr;
	find_instruction_type(top_level_op1, op1_field, decode_func);
	if (decode_func == nullptr) {
		return InstructionType::Undefined;
	}
	else {
		return decode_func(this->_last_raw_instruction);
	}
}

uint32_t A64Decoder::getRawInstruction() {
	return this->_last_raw_instruction;
}
