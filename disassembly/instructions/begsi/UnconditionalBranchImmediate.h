#pragma once

#include <cstdint>

namespace InstructionDefs::Begsi {
	typedef struct UnconditionalBranchImmediate {
	public:
		/**
		 * @var is_with_link
		 * @brief If true, the executor should set X30 to PC+4
		 */
		bool is_with_link;

		/**
		 * @var immediate
		 * @brief Offset from PC, in the range +/- 128MB
		 **/
		int32_t immediate : 26;

		explicit UnconditionalBranchImmediate(uint32_t encoded);
	} UnconditionalBranchImmediateInstruction;
}
