#include "A64Decoder.h"
#include "../numbers/two_complement.h"
#include <utility>
#include <cmath>
#include <stdexcept>

constexpr bool MATCHES_MASK(uint32_t val, uint32_t mask) {
	return (val & mask) == mask;
}

uint32_t read_uint_le(const std::vector<std::byte>& v, const int index) {
	uint32_t value = 0;
	for (int i = 3; i >= 0; --i) {
		value |= static_cast<uint32_t>(v[index + i]) << (i * 8);
	}
	return value;
}

InstructionType decode_data_processing_type(uint32_t raw_instruction) {
	// op0 unused for now
	const uint32_t op0 = raw_instruction >> 29 & 0b11;
	const uint32_t op1 = raw_instruction >> 22 & 0b1111;

	constexpr uint32_t ADD_SUB_MASK = 0b0100;
	if (MATCHES_MASK(op1, ADD_SUB_MASK)) {
		return InstructionType::AddOrSubImmediate;
	}

	return InstructionType::Undefined;
}

InstructionType decode_load_and_store_type(uint32_t raw_instruction) {
	const uint32_t op0 = raw_instruction >> 28 & 0b1111;
	const uint32_t op1 = raw_instruction >> 26 & 1;
	const uint32_t op2 = raw_instruction >> 10 & 0b111111111111111;

	constexpr uint32_t LOAD_STORE_PAIR_MASK_OP0 = 0b0010;
	if (MATCHES_MASK(op0, LOAD_STORE_PAIR_MASK_OP0)) {
		return InstructionType::LoadStoreRegisterPair;
	}

	return InstructionType::Undefined;
}

A64Decoder::A64Decoder(std::vector<std::byte>& code) {
	if (code.size() % sizeof(uint32_t) != 0) {
		throw std::runtime_error("The code bytes must be aligned to 4 bytes");
	}

	this->_code = std::move(code);
	this->_index = 0;
	this->_last_raw_instruction = 0;
}

InstructionType A64Decoder::decode_next() {
	if (this->_index >= this->_code.size()) {
		return InstructionType::Undefined;
	}

	this->_last_raw_instruction = read_uint_le(this->_code, this->_index);
	this->_index += sizeof(uint32_t);

	constexpr uint8_t OP1_MASK = 0b1111;
	uint8_t op1_field = (this->_last_raw_instruction >> 25) & OP1_MASK;

	constexpr uint8_t DATA_PROCESSING_OP1_MASK = 0b1000;
	constexpr uint8_t LOADS_AND_STORES_OP1_MASK = 0b0100;

	if ((op1_field & DATA_PROCESSING_OP1_MASK) == DATA_PROCESSING_OP1_MASK) {
		return decode_data_processing_type(this->_last_raw_instruction);
	}
	else if ((op1_field & LOADS_AND_STORES_OP1_MASK) == LOADS_AND_STORES_OP1_MASK) {
		return decode_load_and_store_type(this->_last_raw_instruction);
	}

	return InstructionType::Undefined;
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
