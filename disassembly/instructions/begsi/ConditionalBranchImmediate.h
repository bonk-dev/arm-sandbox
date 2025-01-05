#pragma once

#include <cstdint>

namespace InstructionDefs::Begsi {
	struct ConditionalBranchImmediate {
	public:
		enum class Condition : uint8_t {
			/**
			 * Branch if equal (Z = 1)
			 */
			Equal = 0b0000,

			/**
			 * Branch if not equal (Z = 0)
			 */
			NotEqual = 0b0001,

			/**
			 * Branch if carry flag is set (unsigned, C = 1)
			 */
			CarrySet = 0b0010,

			/**
			 * Branch if carry flag is not set (unsigned, C = 0)
			 */
			CarryClear = 0b0011,

			/**
			 * Branch if result of an op. is negative (N = 1)
			 */
			Negative = 0b0100,

			/**
			 * Branch if result of an op. was zero or positive (N = 0)
			 */
			NotNegative = 0b0101,

			/**
			 * Branch if overflow flag is set (signed, V = 1)
			 */
			OverflowSet = 0b0110,

			/**
			 * Branch if overflow flag is not set (signed, V = 0)
			 */
			OverflowClear = 0b0111,

			/**
			 * Branch if number is higher (unsigned comp.) (C = 1, Z = 0)
			 */
			Higher = 0b1000,

			/**
			 * Branch if number is lower or same (unsigned comp.) (C = 0 or Z = 1)
			 */
			LowerOrSame = 0b1001,

			/**
			 * Branch if number is great or equal (signed comp.) (N = V)
			 */
			GreaterOrEqual = 0b1010,

			/**
			 * Branch if number is less (signed comp.) (N != V)
			 */
			LessThan = 0b1011,

			/**
			 * Branch if number is greater (signed comp.) (Z = 0, N = V)
			 */
			GreaterThan = 0b1100,

			/**
			 * Branch if number is less or equal (signed comp.) (Z = 1 or N != V)
			 */
			LessOrEqual = 0b1101,

			/**
			 * Always branch, effectively an unconditional branch
			 */
			Always = 0b1110,

			/**
			 * Never branch, effectively a NOP
			 */
			Never = 0b1111
		};

		/**
		 * @var immediate
		 * @brief Offset from PC, in the range +/- 1MB
		 **/
		int32_t immediate;

		/**
		 * @brief If true, hints that this branch will behave very consistently
		 */
		bool isConsistent;

		/**
		 * @brief Specifies what condition has to be met to branch
		 */
		Condition condition;

		explicit ConditionalBranchImmediate(uint32_t encoded);
	};
}
