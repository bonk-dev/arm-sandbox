#include "UnconditionalBranchRegisterExecutor.h"

void
Executors::Begsi::UnconditionalBranchRegisterExecutor::execute(
		const InstructionDefs::Begsi::UnconditionalBranchRegister &instruction, AArch64Cpu& cpu) {
	const virtual_address_t target = cpu.readRegister(instruction.destination_reg, 64);
	this->branchTo(
			target,
			Emulation::BranchType::IndirectCall,
			false,
			instruction.operation_opc == InstructionDefs::Begsi::UnconditionalBranchRegister::OperationOpc::Link,
			cpu);
}
