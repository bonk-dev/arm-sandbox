#pragma once

#include <cstdint>
#include "../../../emulation/emu_types.h"

namespace InstructionDefs::DataProcSimd {
	struct AdvancedSimdCopy {
		enum class CopyType {
			/**
			 * @brief Duplicate a vec. element into a scalar or each element in a vector
			 */
			DuplicateElement,

			/**
			 * @brief Duplicate a gp. register value into a scalar or each element in a vector
			 */
			DuplicateGeneral,

			/**
			 * @brief Signed move (with a sign-extend to 32bit or 64bit) a vector element to a gp. register
			 */
			SignedMove,

			/**
			 * @brief Unsigned move (with a zero-extend to 32bit or 64bit) a vector element to a gp. register
			 */
			UnsignedMove,

			/**
			 * @brief Copies gp. register value to a specific vector element, without zeroing rest of the vector
			 */
			InsertGeneral,

			/**
			 * @brief Copies a vector element to some other specified vector element, without zeroing rest of the vector
			 */
			InsertElement
		};

	private:
		/**
		 * @brief Single bit field from bit 30
		 */
		bool _qField;

		/**
		 * @brief Bit field from bits 20-16
		 */
		uint8_t _imm5 : 5;

		/**
		 * @brief Bit field from bits 14-11
		 */
		uint8_t _imm4 : 4;
	public:
		CopyType copyType;
		regindex_t destinationRegIndex;
		regindex_t sourceRegIndex;

		explicit AdvancedSimdCopy(uint32_t encoded);

		/**
		 * @brief If the copy is done on general purpose registers, use this to get their op size
		 * @return General purpose register operation size
		 */
		[[nodiscard]] uint8_t getGpRegSize() const;

		/**
		 * @brief Finds the base size value, needed for calculating index and element sizes etc.
		 * @return Base size value (lowest set bit pos in imm5<3:0>)
		 */
		[[nodiscard]] unsigned int getBaseSize() const;

		/**
		 * @brief Calculates the source element index which is to be copied
		 * @return Source element index
		 */
		[[nodiscard]] unsigned int getSourceElementIndex() const;

		/**
		 * @brief Calculates the source operand data size
		 * @return Either 64 or 128
		 */
		[[nodiscard]] unsigned int getSourceElementIndexDataSize() const;

		/**
		 * Calculates the element size, if the copy is done on vector elements
		 * @return Size of each element
		 */
		[[nodiscard]] unsigned int getElementSize() const;

		/**
		 * Calculates amount of elements to copy
		 * @return Amount of elements to copy
		 */
		[[nodiscard]] unsigned int getElementCount() const;

		/**
		 * Calculates the final size of data to be written to the vector register
		 * @return
		 */
		[[nodiscard]] unsigned int getDataSize() const;
	};
}