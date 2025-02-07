#include "CompareAndBranchImmediateExecutor.h"

void Executors::Begsi::CompareAndBranchImmediateExecutor::execute(
		const InstructionDefs::Begsi::CompareAndBranchImmediate &instruction, AArch64Cpu &cpu) {
	const uint64_t val = instruction.is64bit
			? cpu.readRegister64(instruction.index, false)
			: cpu.readRegister32(instruction.index, false);
	const bool isNonZero = val != 0;
	const bool doBranch = isNonZero && instruction.branchIfNonZero || !isNonZero && !instruction.branchIfNonZero;

	if (doBranch) {
		this->branchTo(
				cpu.getProgramCounter() + instruction.immediate,
				Emulation::BranchType::DirectBranch,
				true,
				false,
				cpu);
	}
}
