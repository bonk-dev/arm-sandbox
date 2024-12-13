#include "UnconditionalBranchImmediateExecutor.h"

UnconditionalBranchImmediateExecutor::UnconditionalBranchImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu)
	: BranchingExecutor<InstructionDefs::Begsi::UnconditionalBranchImmediate>(cpu) {}

void UnconditionalBranchImmediateExecutor::execute(const InstructionDefs::Begsi::UnconditionalBranchImmediate &instruction) {
	uint64_t pc = this->get_cpu()->getProgramCounter();
	this->branchTo(
			pc + instruction.immediate,
			Emulation::BranchType::IndirectCall,
			false,
			instruction.is_with_link);
}
