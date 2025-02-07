#include "MoveWideImmediateExecutor.h"

void Executors::DataProcImm::MoveWideImmediateExecutor::execute(
		const InstructionDefs::DataProcImm::MoveWideImmediate &instruction, AArch64Cpu& cpu) {
	uint64_t result;
	switch (instruction.op_type) {
		case InstructionDefs::DataProcImm::MoveWideImmediateOpType::Unallocated:
			throw std::runtime_error("Unallocated operation type");
		case InstructionDefs::DataProcImm::MoveWideImmediateOpType::Invert:
			result = ~(instruction.immediate << instruction.left_shift);
			break;
		case InstructionDefs::DataProcImm::MoveWideImmediateOpType::Zero:
			result = instruction.immediate << instruction.left_shift;
			break;
		case InstructionDefs::DataProcImm::MoveWideImmediateOpType::KeepBits:
			result = cpu.readRegister(instruction.destination_reg, instruction.is_64bit ? 64 : 32);
			result &= ~(0b1111111111111111 << instruction.left_shift);
			result |= (instruction.immediate << instruction.left_shift);
			break;
	}

	cpu.writeRegister(instruction.destination_reg, result, instruction.is_64bit ? 64 : 32);
}


