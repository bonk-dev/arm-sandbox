#include "UnconditionalBranchImmediateExecutor.h"

UnconditionalBranchImmediateExecutor::UnconditionalBranchImmediateExecutor(
		const std::shared_ptr<AArch64Cpu> &cpu) : ExecutorBase(cpu) {}

void UnconditionalBranchImmediateExecutor::execute(InstructionDefs::Begsi::UnconditionalBranchImmediate &instruction) {
	uint64_t pc = this->get_cpu()->getProgramCounter();

	if (instruction.is_with_link) {
		this->get_cpu()->writeGpRegister64(30, pc + 4);
	}

	pc += instruction.immediate;
	this->get_cpu()->setProgramCounter(pc);
}
