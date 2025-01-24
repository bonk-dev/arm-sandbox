#include "LoadStoreRegExecutor.h"
#include "../../../disassembly/instructions/loads_and_stores/indexing_helper.h"

void Executors::LoadsAndStores::LoadStoreRegExecutor::execute(
		const InstructionDefs::LoadsAndStores::LoadStoreReg &instruction, AArch64Cpu &cpu) {
	const auto virtual_address = InstructionDefs::IndexingHelpers::calc_next_address(
			instruction.encoding,
			cpu,
			instruction.immediate,
			instruction.baseReg,
			true); // always 64bit op

	if (instruction.isLoad) {
		uint64_t val;
		if (instruction.isSigned) {
			switch (instruction.size) {
				case 8:
					val = cpu.getMemory().read<int8_t>(virtual_address);
					break;
				case 16:
					val = cpu.getMemory().read<int16_t>(virtual_address);
					break;
				case 32:
					val = cpu.getMemory().read<int32_t>(virtual_address);
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

		if (instruction.isUsing64BitReg) {
			cpu.writeRegister64(
					instruction.targetReg, val);
		}
		else {
			cpu.writeRegister32(
					instruction.targetReg, val);
		}
	}
	else {
		if (instruction.isUsing64BitReg) {
			cpu.getMemory().write(
					virtual_address, cpu.readRegister64(instruction.targetReg));
		}
		else {
			cpu.getMemory().write(
					virtual_address, cpu.readRegister32(instruction.targetReg));
		}
	}
}
