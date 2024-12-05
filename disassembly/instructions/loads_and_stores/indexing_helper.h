#pragma once

#include <cstdint>
#include "../IndexingMode.h"
#include "../../../emulation/AArch64Cpu.h"

namespace InstructionDefs::IndexingHelpers {
	template <class T>
	uintptr_t calc_next_address(
			InstructionDefs::IndexingMode mode,
			std::shared_ptr<AArch64Cpu>& cpu,
			T immediate,
			regindex_t base_reg,
			bool is_wide = true
	) {
		uintptr_t virt_addr = is_wide
							  ? cpu->readGpRegister64(base_reg)
							  : cpu->readGpRegister32(base_reg);

		switch (mode) {
			case InstructionDefs::IndexingMode::NonTemporalOffset:
			case InstructionDefs::IndexingMode::SignedOffset:
			case InstructionDefs::IndexingMode::UnsignedOffset:
				virt_addr += immediate;
				break;
			case InstructionDefs::IndexingMode::PostIndex:
				cpu->writeGpRegister64(base_reg, virt_addr + immediate);
				break;
			case InstructionDefs::IndexingMode::PreIndex:
				virt_addr += immediate;
				cpu->writeGpRegister64(base_reg, virt_addr);
				break;
			default:
				throw std::runtime_error("Illegal indexing mode");
		}

		return virt_addr;
	}
}