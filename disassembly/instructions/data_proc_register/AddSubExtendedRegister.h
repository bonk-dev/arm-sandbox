#pragma once
#include <cstdint>
#include "../../../emulation/emu_types.h"

namespace InstructionDefs::DataProcReg {
	struct AddSubExtendedRegister {
	public:
		enum class ExtendVariant : uint8_t {
			/**
			 * @brief Unsigned extend byte (8bit)
			 */
			Uxtb = 0b000,

			/**
			 * @brief Unsigned extend halfword (16bit)
			 */
			Uxth = 0b001,

			/**
			 * @brief Unsigned extend word (32bit)
			 */
			Uxtw = 0b010,

			/**
			 * @brief Unsigned extend doubleword (64bit)
			 */
			Uxtx = 0b011,

			/**
			 * @brief Signed extend byte (8bit)
			 */
			Sxtb = 0b100,

			/**
			 * @brief Signed extend halfword (16bit)
			 */
			Sxth = 0b101,

			/**
			 * @brief Signed extend word (32bit)
			 */
			Sxtw = 0b110,

			/**
			 * @brief Signed extend doubleword (64bit)
			 */
			Sxtx = 0b111
		};

		/**
		 * @brief Whether read/writes are to be performed on 64bit registers (or 32bit registers)
		 */
		bool is64Bit;

		/**
		 * @brief Whether the arithmetic operation should be a subtraction (if false, it is going to be addition)
		 */
		bool subtract;

		/**
		 * @brief Whether the NZCV flags should be set
		 */
		bool setFlags;

		/**
		 * @brief Auxiliary field, must be equal to 00
		 */
		uint8_t optField : 2;

		/**
		 * @brief Index of the second source general purpose register (must not be SP)
		 */
		regindex_t secondSourceReg;

		/**
		 * @brief What kind of extension should be done on the second source operand
		 */
		ExtendVariant extendVariant;

		/**
		 * @brief Amount of LSL to be applied after extension
		 */
		uint8_t shiftAmount : 3;

		/**
		 * @brief Index of the second first general purpose register (can be SP)
		 */
		regindex_t firstSourceReg;

		/**
		 * @brief Index of the destination register (must not be SP)
		 */
		regindex_t destinationReg;

		explicit AddSubExtendedRegister(uint32_t encoded);
	};
}