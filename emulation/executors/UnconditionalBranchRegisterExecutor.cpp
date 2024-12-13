#include "UnconditionalBranchRegisterExecutor.h"

UnconditionalBranchRegisterExecutor::UnconditionalBranchRegisterExecutor(const std::shared_ptr<AArch64Cpu> &cpu)
		: BranchingExecutor<InstructionDefs::Begsi::UnconditionalBranchRegister>(cpu) {}

void
UnconditionalBranchRegisterExecutor::execute(const InstructionDefs::Begsi::UnconditionalBranchRegister &instruction) {
	const virtual_address_t target = this->get_cpu()->readGpRegister64(instruction.destination_reg);
	this->branchTo(
			target,
			Emulation::BranchType::IndirectCall,
			false,
			instruction.branch_with_link);
}
