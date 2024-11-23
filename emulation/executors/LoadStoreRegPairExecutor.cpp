#include "LoadStoreRegPairExecutor.h"

LoadStoreRegPairExecutor::LoadStoreRegPairExecutor(const std::shared_ptr<AArch64Cpu> &cpu) : ExecutorBase(cpu) {}

uintptr_t LoadStoreRegPairExecutor::calc_next_address(InstructionDefs::LoadsAndStores::LoadStoreRegisterPairInstruction& instruction) const {
	const auto cpu = this->get_cpu().get();
	uintptr_t virt_addr = instruction.is_wide
						  ? cpu->read_gp_register_64(instruction.base_reg)
						  : cpu->read_gp_register_32(instruction.base_reg);

	switch (instruction.encoding) {
		case InstructionDefs::LoadsAndStores::LoadStorePairEncoding::NonTemporalOffset:
		case InstructionDefs::LoadsAndStores::LoadStorePairEncoding::SignedOffset:
			virt_addr += instruction.immediate_value;
			break;
		case InstructionDefs::LoadsAndStores::LoadStorePairEncoding::PostIndex:
			cpu->write_gp_register_64(instruction.base_reg, virt_addr + instruction.immediate_value);
			break;
		case InstructionDefs::LoadsAndStores::LoadStorePairEncoding::PreIndex:
			virt_addr += instruction.immediate_value;
			cpu->write_gp_register_64(instruction.base_reg, virt_addr);
			break;
	}

	return virt_addr;
}

void LoadStoreRegPairExecutor::execute(InstructionDefs::LoadsAndStores::LoadStoreRegisterPairInstruction& instruction) {
	if (instruction.is_simd) {
		throw std::runtime_error("SIMD operations are not implemented");
	}

	const auto cpu = this->get_cpu().get();

	const uint8_t reg_indexes[2] = {instruction.first_reg_index, instruction.second_reg_index};
	for (uint8_t reg_index : reg_indexes) {
		const uintptr_t virtual_address = this->calc_next_address(instruction);
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
				cpu->get_memory().write_uint64(virtual_address, cpu->read_gp_register_64(reg_index));
			}
			else {
				cpu->get_memory().write_uint32(virtual_address, cpu->read_gp_register_32(reg_index));
			}
		}
	}
}
