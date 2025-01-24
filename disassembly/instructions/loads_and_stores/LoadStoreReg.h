#pragma once
#include <cstdint>
#include "../IndexingMode.h"
#include "../../../emulation/emu_types.h"

namespace InstructionDefs::LoadsAndStores {
	struct LoadStoreReg {
	public:
		uint8_t size;
		bool isSimd;
		InstructionDefs::IndexingMode encoding;

		/**
		 * @brief If true, this is an unscaled imm load/store. If false, this is an unprivileged load/store
		 */
		bool isUnscaledImm;

		bool isLoad;
		bool isSigned;
		bool isUsing64BitReg;

		regindex_t targetReg;
		regindex_t baseReg;
		int16_t immediate : 9;

		explicit LoadStoreReg(uint32_t encoded);
	};
}