#include "A64Decoder.h"
#include <utility>
#include <cmath>
#include <stdexcept>

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
	if ((op1 & ADD_SUB_MASK) == ADD_SUB_MASK) {
		return InstructionType::AddOrSubImmediate;
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

	this->_last_raw_instruction = read_uint_le(this->_code, this->_index++);

	constexpr uint8_t OP1_MASK = 0b1111;
	uint8_t op1_field = (this->_last_raw_instruction >> 25) & OP1_MASK;

	constexpr uint8_t DATA_PROCESSING_OP1_MASK = 0b1000;
	if ((op1_field & DATA_PROCESSING_OP1_MASK) == DATA_PROCESSING_OP1_MASK) {
		return decode_data_processing_type(this->_last_raw_instruction);
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
