#pragma once

#include "ExecutorBase.h"
#include "../../disassembly/instructions/UnconditionalBranchImmediate.h"

class UnconditionalBranchImmediateExecutor : ExecutorBase {
public:
	explicit UnconditionalBranchImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(InstructionDefs::UnconditionalBranchImmediate& instruction);
};
