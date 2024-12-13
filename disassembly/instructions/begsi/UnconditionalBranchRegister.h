#pragma once
#include <cstdint>
#include "../../../emulation/emu_types.h"

namespace InstructionDefs::Begsi {
	typedef struct UnconditionalBranchRegister {
	public:
		/**
		 * @brief If true, an executor should set X30 to PC+4
		 */
		bool branch_with_link;

		/**
		 * @brief Index of the GP register holding the addr to be branched to
		 */
		regindex_t destination_reg;

		explicit UnconditionalBranchRegister(uint32_t encoded);
	} UnconditionalBranchRegister;
}