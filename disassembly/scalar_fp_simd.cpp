#include "scalar_fp_simd.h"
#include "instructions/init_helpers.h"
#include <map>
#include "decoder_utils.h"

namespace {
	using mask_values_t = Decoding::mask_values_t;

	// Top-level -> Data processing (sc. fp and adv. SIMD) -> (op0) -> (op1) -> (op2) -> (op3)
	std::map<mask_values_t, std::map<mask_values_t, std::map<mask_values_t, std::map<mask_values_t, InstructionType>>>> data_proc_simd_op0123{
			{
				// op0: 0xx0
				mask_values_t(0b1001, 0b0000),
				{
					{
						// op1: 00
						mask_values_t(0b11, 0b00),
						{
							{
								// op2: 00xx
								mask_values_t(0b1100, 0b0000),
								{
									{
										// op3: xxx0xxxx1
										mask_values_t(0b000100001, 0b000000001), InstructionType::AdvancedSimdCopy
									}
								}
							}
						}
					}
				}
			}
	};
}

namespace Decoding {
	InstructionType decode_data_processing_fp_simd(uint32_t rawInstruction) {
		const uint32_t op0 = i::val(rawInstruction, 4, 28);
		const uint32_t op1 = i::val(rawInstruction, 2, 23);
		const uint32_t op2 = i::val(rawInstruction, 4, 19);
		const uint32_t op3 = i::val(rawInstruction, 9, 10);

		auto op1_map = Decoding::find_instruction_type(data_proc_simd_op0123, op0);
		if (op1_map != nullptr) {
			auto op2_map = Decoding::find_instruction_type(*op1_map, op1);
			if (op2_map != nullptr) {
				auto op3_map = Decoding::find_instruction_type(*op2_map, op2);
				if (op3_map != nullptr) {
					return Decoding::find_instruction_type_fast(*op3_map, op3);
				}
			}
		}

		return InstructionType::Undefined;
	}
}