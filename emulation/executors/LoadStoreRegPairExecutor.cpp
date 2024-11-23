#include "LoadStoreRegPairExecutor.h"
#include "../../disassembly/instructions/loads_and_stores/indexing_helper.h"

LoadStoreRegPairExecutor::LoadStoreRegPairExecutor(const std::shared_ptr<AArch64Cpu> &cpu) : ExecutorBase(cpu) {}

void LoadStoreRegPairExecutor::execute(InstructionDefs::LoadsAndStores::LoadStoreRegisterPairInstruction& instruction) {
	if (instruction.is_simd) {
		throw std::runtime_error("SIMD operations are not implemented");
	}

	const auto cpu = this->get_cpu().get();

	const uint8_t reg_indexes[2] = {instruction.first_reg_index, instruction.second_reg_index};
	for (uint8_t reg_index : reg_indexes) {
		const auto virtual_address = InstructionDefs::IndexingHelpers::calc_next_address(
				instruction.encoding,
				this->get_cpu(),
				instruction.immediate_value,
				instruction.base_reg,
				instruction.is_wide);

		if (instruction.is_load) {
			if (instruction.is_wide) {
				cpu->write_gp_register_64(reg_index, cpu->get_memory().read_uint64(virtual_address));
			}
			else {
				cpu->write_gp_register_32(reg_index, cpu->get_memory().read_uint32(virtual_address));
			}
		}
		else {
			if (instruction.is_wide) {
				cpu->get_memory().write(virtual_address, cpu->read_gp_register_64(reg_index));
			}
			else {
				cpu->get_memory().write(virtual_address, cpu->read_gp_register_32(reg_index));
			}
		}
	}
}
