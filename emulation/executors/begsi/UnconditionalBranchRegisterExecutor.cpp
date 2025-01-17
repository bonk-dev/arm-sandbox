#include "UnconditionalBranchRegisterExecutor.h"

void
Executors::Begsi::UnconditionalBranchRegisterExecutor::execute(
		const InstructionDefs::Begsi::UnconditionalBranchRegister &instruction, AArch64Cpu& cpu) {
	const virtual_address_t target = cpu.readRegister64(instruction.destination_reg);
	this->branchTo(
			target,
			Emulation::BranchType::IndirectCall,
			false,
			instruction.operation_opc == InstructionDefs::Begsi::UnconditionalBranchRegister::OperationOpc::Link,
			cpu);
}
