#include "MoveWideImmediateExecutor.h"

MoveWideImmediateExecutor::MoveWideImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu) : ExecutorBase(cpu) {}

void MoveWideImmediateExecutor::execute(InstructionDefs::MoveWideImmediate &instruction) {
	uint64_t result;
	switch (instruction.op_type) {
		case InstructionDefs::MoveWideImmediateOpType::Unallocated:
			throw std::runtime_error("Unallocated operation type");
		case InstructionDefs::MoveWideImmediateOpType::Invert:
			result = ~(instruction.immediate << instruction.left_shift);
			break;
		case InstructionDefs::MoveWideImmediateOpType::Zero:
			result = instruction.immediate << instruction.left_shift;
			break;
		case InstructionDefs::MoveWideImmediateOpType::KeepBits:
			result = instruction.is_64bit
					? this->get_cpu()->read_gp_register_64(instruction.destination_reg)
					: this->get_cpu()->read_gp_register_32(instruction.destination_reg);
			result &= ~(0b1111111111111111 << instruction.left_shift);
			result |= (instruction.immediate << instruction.left_shift);
			break;
	}

	if (instruction.is_64bit) {
		this->get_cpu()->write_gp_register_64(instruction.destination_reg, result);
	}
	else {
		this->get_cpu()->write_gp_register_32(instruction.destination_reg, result);
	}
}


