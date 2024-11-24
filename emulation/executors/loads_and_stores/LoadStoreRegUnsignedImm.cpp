#include "LoadStoreRegUnsignedImm.h"
#include "../../exceptions/FeatureFpNotImplemented.h"
#include "../../../disassembly/instructions/loads_and_stores/indexing_helper.h"

Executors::LoadsAndStores::LoadStoreRegUnsignedImm::LoadStoreRegUnsignedImm(const std::shared_ptr<AArch64Cpu> &cpu) : ExecutorBase(cpu) {}

void Executors::LoadsAndStores::LoadStoreRegUnsignedImm::execute(InstructionDefs::LoadsAndStores::LoadStoreRegUnsignedImm& instruction) {
	if (instruction.is_simd) {
		throw Exceptions::FeatureFpNotImplemented("LoadStoreRegUnsignedImm");
	}
	if (instruction.get_is_prefetch()) {
		// do nothing
	}

	const int imm = instruction.indexing_mode == InstructionDefs::IndexingMode::UnsignedOffset
			? (instruction.unsigned_imm)
			: instruction.signed_imm9;
	const auto virtual_address = InstructionDefs::IndexingHelpers::calc_next_address(
			instruction.indexing_mode,
			this->get_cpu(),
			imm,
			instruction.base_reg,
			true); // always 64bit op

	if (instruction.is_load) {
		uint64_t val;
		if (instruction.is_signed) {
			switch (instruction.size) {
				case 8:
					val = this->get_cpu()->get_memory().read<int8_t>(virtual_address);
					break;
				case 16:
					val = this->get_cpu()->get_memory().read<int16_t>(virtual_address);
					break;
				case 32:
					val = this->get_cpu()->get_memory().read<int32_t>(virtual_address);
					break;
				case 64:
					val = this->get_cpu()->get_memory().read<int64_t>(virtual_address);
					break;
				default:
					throw std::runtime_error("Invalid data size");
			}
		}
		else {
			switch (instruction.size) {
				case 8:
					val = this->get_cpu()->get_memory().read<uint8_t>(virtual_address);
					break;
				case 16:
					val = this->get_cpu()->get_memory().read<uint16_t>(virtual_address);
					break;
				case 32:
					val = this->get_cpu()->get_memory().read<uint32_t>(virtual_address);
					break;
				case 64:
					val = this->get_cpu()->get_memory().read<uint64_t>(virtual_address);
					break;
				default:
					throw std::runtime_error("Invalid data size");
			}
		}

		if (instruction.is_using_64bit_reg) {
			this->get_cpu()->write_gp_register_64(
					instruction.src_dst_reg, val);
		}
		else {
			this->get_cpu()->write_gp_register_32(
					instruction.src_dst_reg, val);
		}
	}
	else {
		if (instruction.is_using_64bit_reg) {
			this->get_cpu()->get_memory().write(
					virtual_address, this->get_cpu()->read_gp_register_64(instruction.src_dst_reg));
		}
		else {
			this->get_cpu()->get_memory().write(
					virtual_address, this->get_cpu()->read_gp_register_32(instruction.src_dst_reg));
		}
	}
}
