#include "MoveWideImmediateExecutor.h"

MoveWideImmediateExecutor::MoveWideImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu) : ExecutorBase(cpu) {}

void MoveWideImmediateExecutor::execute(InstructionDefs::DataProcImm::MoveWideImmediate &instruction) {
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
			result = instruction.is_64bit
					? this->get_cpu()->readGpRegister64(instruction.destination_reg)
					: this->get_cpu()->readGpRegister32(instruction.destination_reg);
			result &= ~(0b1111111111111111 << instruction.left_shift);
			result |= (instruction.immediate << instruction.left_shift);
			break;
	}

	if (instruction.is_64bit) {
		this->get_cpu()->writeGpRegister64(instruction.destination_reg, result);
	}
	else {
		this->get_cpu()->writeGpRegister32(instruction.destination_reg, result);
	}
}


