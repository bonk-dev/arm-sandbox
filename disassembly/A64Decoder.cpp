#include <cmath>
#include <map>
#include "A64Decoder.h"
#include "decoder_utils.h"
#include "data_processing_imm.h"
#include "branches_exc_sys.h"
#include "data_processing_register.h"
#include "scalar_fp_simd.h"
#include "load_and_store.h"

namespace {
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
			{Decoding::mask_values_t(0b1110, 0b1010), &Decoding::decode_branches_exc_sys},
			{Decoding::mask_values_t(0b0111, 0b0101), &Decoding::decode_data_processing_register_type},
			{Decoding::mask_values_t(0b0111, 0b0111), &Decoding::decode_data_processing_fp_simd},
			{Decoding::mask_values_t(0b0101, 0b0100), &Decoding::decode_load_and_store_type}
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
