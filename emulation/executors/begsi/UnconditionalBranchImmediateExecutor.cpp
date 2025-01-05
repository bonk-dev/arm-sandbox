#include "UnconditionalBranchImmediateExecutor.h"

void Executors::Begsi::UnconditionalBranchImmediateExecutor::execute(
		const InstructionDefs::Begsi::UnconditionalBranchImmediate &instruction, AArch64Cpu& cpu) {
	uint64_t pc = cpu.getProgramCounter();
	this->branchTo(
			pc + instruction.immediate,
			Emulation::BranchType::IndirectCall,
			false,
			instruction.is_with_link,
			cpu);
}
