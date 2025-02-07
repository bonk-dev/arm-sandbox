#pragma once

#include <cstdint>
#include "../../../emulation/emu_types.h"

namespace InstructionDefs::Begsi {
	struct CompareAndBranchImmediate {
	public:
		/**
		 * @brief If true, reads/writes are performed on 64bit registers.
		 */
		bool is64bit;

		/**
		 * @brief If true, the branching should be performed if the value was non-zero. \n
		 * If false, then it should be done if the value WAS zero.
		 */
		bool branchIfNonZero;

		/**
		 * @brief Offset from PC, in the range +/- 1MB
		 **/
		int32_t immediate : 21; // 19 + 2 to fit the shifted value

		/**
		 * @brief Register of which the value is to be tested
		 */
		regindex_t index;

		explicit CompareAndBranchImmediate(uint32_t encoded);
	};
}
