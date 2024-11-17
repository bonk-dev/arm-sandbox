#include "UnconditionalBranchImmediateExecutor.h"

UnconditionalBranchImmediateExecutor::UnconditionalBranchImmediateExecutor(
		const std::shared_ptr<AArch64Cpu> &cpu) : ExecutorBase(cpu) {}

void UnconditionalBranchImmediateExecutor::execute(UnconditionalBranchImmediateInstruction &instruction) {
	uint64_t pc = this->get_cpu()->get_program_counter();

	if (instruction.is_with_link) {
		this->get_cpu()->write_gp_register_64(30, pc + 4);
	}

	pc += instruction.immediate;
	this->get_cpu()->set_program_counter(pc);
}
