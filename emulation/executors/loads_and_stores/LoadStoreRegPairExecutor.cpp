#include "LoadStoreRegPairExecutor.h"
#include "../../../disassembly/instructions/loads_and_stores/indexing_helper.h"

void Executors::LoadsAndStores::LoadStoreRegPairExecutor::execute(
		const InstructionDefs::LoadsAndStores::LoadStoreRegisterPair& instruction, AArch64Cpu& cpu) {
	if (instruction.is_simd) {
		throw std::runtime_error("SIMD operations are not implemented");
	}

	auto virtual_address = InstructionDefs::IndexingHelpers::calc_next_address(
				instruction.encoding,
				cpu,
				instruction.immediate_value,
				instruction.base_reg,
				instruction.is_wide);

	const uint8_t reg_indexes[2] = {instruction.first_reg_index, instruction.second_reg_index};
	for (uint8_t reg_index : reg_indexes) {
		if (instruction.is_load) {
			if (instruction.is_wide) {
				cpu.writeRegister64(reg_index, cpu.getMemory().read<uint64_t>(virtual_address));
			}
			else {
				cpu.writeRegister32(reg_index, cpu.getMemory().read<uint32_t>(virtual_address));
			}
		}
		else {
			if (instruction.is_wide) {
				cpu.getMemory().write(virtual_address, cpu.readRegister64(reg_index));
			}
			else {
				cpu.getMemory().write(virtual_address, cpu.readRegister32(reg_index));
			}
		}

		virtual_address += instruction.is_wide ? sizeof(uint64_t) : sizeof(uint32_t);
	}
}
