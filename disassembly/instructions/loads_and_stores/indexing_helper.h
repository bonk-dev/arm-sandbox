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
		uintptr_t virt_addr = is_wide
							  ? cpu.readRegister64(base_reg, true)
							  : cpu.readRegister32(base_reg, true);

		switch (mode) {
			case InstructionDefs::IndexingMode::NonTemporalOffset:
			case InstructionDefs::IndexingMode::SignedOffset:
			case InstructionDefs::IndexingMode::UnsignedOffset:
				virt_addr += immediate;
				break;
			case InstructionDefs::IndexingMode::PostIndex:
				cpu.writeRegister64(base_reg, virt_addr + immediate, true);
				break;
			case InstructionDefs::IndexingMode::PreIndex:
				virt_addr += immediate;
				cpu.writeRegister64(base_reg, virt_addr, true);
				break;
			default:
				throw std::runtime_error("Illegal indexing mode");
		}

		return virt_addr;
	}
}