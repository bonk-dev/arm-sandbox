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
		{ mask_values_t(0b1100, 0b0000), InstructionType::PcRelativeAddressing }
};
InstructionType decode_data_processing_type(uint32_t raw_instruction) {
	// op0 unused for now
	const uint32_t op0 = raw_instruction >> 29 & 0b11;
	const uint32_t op1 = raw_instruction >> 22 & 0b1111;

	InstructionType result = InstructionType::Undefined;
	find_instruction_type(data_proc_op1, op1, result);
	return result;
}

// Top-level -> Load and store -> (op0 field)
static std::map<mask_values_t , InstructionType> load_and_store_op0 {
		{ mask_values_t(0b0011, 0b0010), InstructionType::LoadStoreRegisterPair },
};
InstructionType decode_load_and_store_type(uint32_t raw_instruction) {
	const uint32_t op0 = raw_instruction >> 28 & 0b1111;
	const uint32_t op1 = raw_instruction >> 26 & 1;
	const uint32_t op2 = raw_instruction >> 10 & 0b111111111111111;

	InstructionType result = InstructionType::Undefined;
	find_instruction_type(load_and_store_op0, op0, result);
	return result;
}

A64Decoder::A64Decoder(std::vector<std::byte>& code) {
	if (code.size() % sizeof(uint32_t) != 0) {
		throw std::runtime_error("The code bytes must be aligned to 4 bytes");
	}

	this->_code = std::move(code);
	this->_index = 0;
	this->_last_raw_instruction = 0;
}

typedef InstructionType (*decode_sublevel_instruction_t)(uint32_t);
static std::map<mask_values_t, decode_sublevel_instruction_t> top_level_op1 {
		{ mask_values_t(0b1110, 0b1000), &decode_data_processing_type },
		{ mask_values_t(0b0101, 0b0100), &decode_load_and_store_type }
};
InstructionType A64Decoder::decode_next() {
	if (this->_index >= this->_code.size()) {
		return InstructionType::Undefined;
	}

	this->_last_raw_instruction = read_uint_le(this->_code, this->_index);
	this->_index += sizeof(uint32_t);

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

AddImmediateInstruction A64Decoder::decode_add_immediate() const {
	constexpr uint32_t DEST_MASK = 0b11111;
	constexpr uint32_t SOURCE_MASK = 0b11111;
	constexpr uint32_t IMM12_MASK = 0b111111111111;

	const bool is_64bit = this->_last_raw_instruction >> 31 & 1;
	const bool is_sub = this->_last_raw_instruction >> 30 & 1;
	const bool set_bits = this->_last_raw_instruction >> 29 & 1;
	const bool shift_12 = this->_last_raw_instruction >> 30 & 1;

	const uint16_t imm12_val = (this->_last_raw_instruction >> 10) & IMM12_MASK;
	const uint8_t source = (this->_last_raw_instruction >> 5) & SOURCE_MASK;
	const uint8_t destination = this->_last_raw_instruction & DEST_MASK;

	AddImmediateInstruction details {
		is_64bit,
		is_sub,
		set_bits,
		shift_12,
		imm12_val,
		source,
		destination
	};
	return details;
}

FormPcRelAddressInstruction A64Decoder::decode_form_pc_rel_addr_instruction() const {
	const uint32_t raw = this->_last_raw_instruction;

	const bool rel_to_4kb = raw >> 31 & 1;
	const uint32_t imm_lo = raw >> 29 & 0b11;
	const uint32_t imm_hi = raw >> 5 & 0b1111111111111111111; // 19 bits
	const regindex_t reg_destination_index = raw & 0b11111;

	const uint32_t joined_imm = (imm_hi << 2) | imm_lo;
	const int32_t signed_imm = signed_21_bit { static_cast<int>(joined_imm) }.val;

	return FormPcRelAddressInstruction {
		rel_to_4kb,
		reg_destination_index,
		signed_imm
	};
}

LoadStoreRegisterPairInstruction A64Decoder::decode_load_store_register_pair_instruction() const {
	const uint32_t raw = this->_last_raw_instruction;

	const bool is_wide = raw >> 31 & 1;
	const bool is_simd = raw >> 26 & 1;
	const auto encoding = static_cast<LoadStorePairEncoding>((raw >> 23) & 0b111);
	const bool is_load = raw >> 22 & 1;

	constexpr uint8_t REGISTER_MASK = 0b11111;
	const uint8_t first_reg = raw & REGISTER_MASK;
	const uint8_t second_reg = raw >> 10 & REGISTER_MASK;
	const uint8_t base_reg = raw >> 5 & REGISTER_MASK;

	const uint8_t raw_imm7 = raw >> 15 & 0b1111111;
	const int signed_imm7 = signed_7_bit {raw_imm7 }.val;

	const int actual_imm_value = is_wide
			? signed_imm7 * 8
			: signed_imm7 * 4;

	return LoadRegisterPairInstruction{
		is_wide,
		is_simd,
		encoding,
		is_load,

		first_reg,
		second_reg,
		base_reg,
		static_cast<int16_t>(actual_imm_value)
	};
}
