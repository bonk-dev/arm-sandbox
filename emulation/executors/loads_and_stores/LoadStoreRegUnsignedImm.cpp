#include "LoadStoreRegUnsignedImm.h"
#include "../../exceptions/FeatureFpNotImplemented.h"
#include "../../../disassembly/instructions/loads_and_stores/indexing_helper.h"

void Executors::LoadsAndStores::LoadStoreRegUnsignedImm::execute(
		const InstructionDefs::LoadsAndStores::LoadStoreRegUnsignedImm& instruction, AArch64Cpu& cpu) {
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
			cpu,
			imm,
			instruction.base_reg,
			true); // always 64bit op

	if (instruction.is_load) {
		uint64_t val;
		if (instruction.is_signed) {
			// static_cast avoids sign extending to 64bits
			switch (instruction.size) {
				case 8:
					val = static_cast<uint8_t>(cpu.getMemory().read<int8_t>(virtual_address));
					break;
				case 16:
					val = static_cast<uint16_t>(cpu.getMemory().read<int16_t>(virtual_address));
					break;
				case 32:
					val = static_cast<uint32_t>(cpu.getMemory().read<int32_t>(virtual_address));
					break;
				case 64:
					val = cpu.getMemory().read<int64_t>(virtual_address);
					break;
				default:
					throw std::runtime_error("Invalid data size");
			}
		}
		else {
			switch (instruction.size) {
				case 8:
					val = cpu.getMemory().read<uint8_t>(virtual_address);
					break;
				case 16:
					val = cpu.getMemory().read<uint16_t>(virtual_address);
					break;
				case 32:
					val = cpu.getMemory().read<uint32_t>(virtual_address);
					break;
				case 64:
					val = cpu.getMemory().read<uint64_t>(virtual_address);
					break;
				default:
					throw std::runtime_error("Invalid data size");
			}
		}

		if (instruction.is_using_64bit_reg) {
			cpu.writeGpRegister64(
					instruction.src_dst_reg, val);
		}
		else {
			cpu.writeGpRegister32(
					instruction.src_dst_reg, val);
		}
	}
	else {
		if (instruction.is_using_64bit_reg) {
			cpu.getMemory().write(
					virtual_address, cpu.readGpRegister64(instruction.src_dst_reg));
		}
		else {
			cpu.getMemory().write(
					virtual_address, cpu.readGpRegister32(instruction.src_dst_reg));
		}
	}
}
