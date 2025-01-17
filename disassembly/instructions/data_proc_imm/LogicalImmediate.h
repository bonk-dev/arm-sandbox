#pragma once

#include "../../../emulation/emu_types.h"
#include "../LogicalOperation.h"

namespace InstructionDefs::DataProcImm {
	struct LogicalImmediate {
	public:
		/**
		 * @brief If true, executor should operate on 64-bit registers (Xn)
		 */
		bool is64Bit;

		/**
		 * @brief What kind of bitwise operation should be done on the two register values.
		 */
		InstructionDefs::LogicalOperation operation;

		/**
		 * @brief The immediate bitmask to be applied to the value of the source register
		 */
		uint16_t bitmask;
		regindex_t destinationReg;
		regindex_t sourceReg;

		explicit LogicalImmediate(uint32_t encoded);
	};
}
