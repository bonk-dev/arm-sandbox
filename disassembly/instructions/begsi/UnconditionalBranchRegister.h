#pragma once
#include <cstdint>
#include "../../../emulation/emu_types.h"

namespace InstructionDefs::Begsi {
	typedef struct UnconditionalBranchRegister {
	public:
		/**
		 * @brief Interpretation of the OPC field
		 */
		enum class OperationOpc {
			/**
			 * Normal branch
			 */
			Normal = 0b0000,
			/**
			 * Executor should set X30 to PC+4
			 */
			Link = 0b0001,
			/**
			 * Executor should set branch to X30
			 */
			Return = 0b0010
		};

		/**
		 * @brief Interpretaion of the OPC field
		 */
		OperationOpc operation_opc;

		/**
		 * @brief Index of the GP register holding the addr to be branched to
		 */
		regindex_t destination_reg;

		explicit UnconditionalBranchRegister(uint32_t encoded);
		UnconditionalBranchRegister(OperationOpc opc, regindex_t regindex);

		uint32_t encode() const;
	} UnconditionalBranchRegister;
}