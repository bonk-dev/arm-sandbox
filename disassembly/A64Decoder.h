#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "../emulation/emu_types.h"

enum class InstructionType {
	AddOrSubImmediate,
	UnconditionalBranchImmediate,
	PcRelativeAddressing,
	LoadStoreRegisterPair,
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

typedef struct FormPcRelAddressInstruction {
	bool rel_to_4kb_page;

	regindex_t destination_reg_index;
	int32_t immediate;
} FormPcRelAddressInstruction;

enum class LoadStorePairEncoding : uint8_t {
	NonTemporalOffset = 0b000,
	PostIndex = 0b001,
	PreIndex = 0b011,
	SignedOffset = 0b010
};

typedef struct LoadRegisterPairInstruction {
	/*
	 * For non-SIMD: if true, the instruction is 64-bit, 32-bit otherwise
	 * For SIMD: if true, the instruction is 128-bit, 64-bit otherwise
	 */
	bool is_wide;
	bool is_simd;
	LoadStorePairEncoding encoding;
	bool is_load;

	uint8_t first_reg_index;
	uint8_t second_reg_index;
	uint8_t base_reg;
	int16_t immediate_value;
} LoadStoreRegisterPairInstruction;

class A64Decoder {
private:
	std::vector<std::byte> _code;
	int _index;
	uint32_t _last_raw_instruction;
public:
	explicit A64Decoder(std::vector<std::byte>& code);
	InstructionType decode_next();

	[[nodiscard]] AddImmediateInstruction decode_add_immediate() const;
	[[nodiscard]] FormPcRelAddressInstruction decode_form_pc_rel_addr_instruction() const;

	[[nodiscard]] LoadStoreRegisterPairInstruction decode_load_store_register_pair_instruction() const;
};