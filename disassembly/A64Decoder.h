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

typedef struct UnconditionalBranchImmediateInstruction {
	/**
	 * @var is_with_link
	 * @brief If true, the executor should set X30 to PC+4
	 */
	bool is_with_link;

	/**
	 * @var immediate
	 * @brief Offset from PC, in the range +/- 128MB
	 **/
	int32_t immediate;
} UnconditionalBranchImmediateInstruction;

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

	template<class InstructionDetailsT>
	[[nodiscard]] InstructionDetailsT decode_details() const {
		return InstructionDetailsT(this->_last_raw_instruction);
	}

	[[nodiscard]] UnconditionalBranchImmediateInstruction decode_unconditional_branch_instruction() const;

	[[nodiscard]] LoadStoreRegisterPairInstruction decode_load_store_register_pair_instruction() const;
};