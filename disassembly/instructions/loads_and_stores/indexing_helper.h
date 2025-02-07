#pragma once

#include <cstdint>
#include "../IndexingMode.h"
#include "../../../emulation/AArch64Cpu.h"

namespace InstructionDefs::IndexingHelpers {
	template <class T>
	uintptr_t calc_next_address(
			InstructionDefs::IndexingMode mode,
			AArch64Cpu& cpu,
			T immediate,
			regindex_t base_reg,
			bool is_wide = true
	) {
		uintptr_t virt_addr = cpu.readRegisterSp(base_reg, is_wide ? 64 : 32);
		switch (mode) {
			case InstructionDefs::IndexingMode::NonTemporalOffset:
			case InstructionDefs::IndexingMode::SignedOffset:
			case InstructionDefs::IndexingMode::UnsignedOffset:
				virt_addr += immediate;
				break;
			case InstructionDefs::IndexingMode::PostIndex:
				cpu.writeRegisterSp(base_reg, virt_addr + immediate, 64);
				break;
			case InstructionDefs::IndexingMode::PreIndex:
				virt_addr += immediate;
				cpu.writeRegisterSp(base_reg, virt_addr, 64);
				break;
			default:
				throw std::runtime_error("Illegal indexing mode");
		}

		return virt_addr;
	}
}