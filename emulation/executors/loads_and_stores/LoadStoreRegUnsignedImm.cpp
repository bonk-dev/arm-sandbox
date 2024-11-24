#include "LoadStoreRegUnsignedImm.h"
#include "../../exceptions/FeatureFpNotImplemented.h"
#include "../../../disassembly/instructions/loads_and_stores/indexing_helper.h"

Executors::LoadsAndStores::LoadStoreRegUnsignedImm::LoadStoreRegUnsignedImm(const std::shared_ptr<AArch64Cpu> &cpu) : ExecutorBase(cpu) {}

void Executors::LoadsAndStores::LoadStoreRegUnsignedImm::execute(InstructionDefs::LoadsAndStores::LoadStoreRegUnsignedImm& instruction) {
	if (instruction.is_simd) {
		throw Exceptions::FeatureFpNotImplemented("LoadStoreRegUnsignedImm");
	}

//	const int imm = instruction.indexing_mode == InstructionDefs::IndexingMode::UnsignedOffset
//			? (instruction.unsigned_imm << 1)
//			: instruction.signed_imm9;
//	const auto virtual_address = InstructionDefs::IndexingHelpers::calc_next_address(
//			instruction.indexing_mode,
//			this->get_cpu(),
//			imm,
//			instruction.base_reg,
//			true); // always 64bit op
//
//	if (instruction.is_load) {
//		if (instruction.is_wide) {
//			cpu->write_gp_register_64(reg_index, cpu->get_memory().read_uint64(virtual_address));
//		}
//		else {
//			cpu->write_gp_register_32(reg_index, cpu->get_memory().read_uint32(virtual_address));
//		}
//	}
//	else {
//		if (instruction.is_wide) {
//			cpu->get_memory().write(virtual_address, cpu->read_gp_register_64(reg_index));
//		}
//		else {
//			cpu->get_memory().write(virtual_address, cpu->read_gp_register_32(reg_index));
//		}
//	}

	throw std::runtime_error("LDR/STR Not implemented");
}
