#pragma once
#include <cstdint>
#include <vector>

enum class InstructionType {
	AddImmediate,
	Undefined = 0xFFFFFFF
};

typedef struct AddImmediateInstruction {
	bool is_64bit;
	bool is_subtraction;
	bool set_flags;
	bool shift_12;
	uint16_t immediate; // 0-4095, 12 bits

	uint8_t destination_reg_index;
	uint8_t source_reg_index;
} AddImmediateInstruction;

class A64Decoder {
private:
	std::vector<std::byte> _code;
	int _index;
	uint32_t _last_raw_instruction;
public:
	explicit A64Decoder(std::vector<std::byte>& code);
	InstructionType decode_next();

	AddImmediateInstruction decode_add_immediate();
};