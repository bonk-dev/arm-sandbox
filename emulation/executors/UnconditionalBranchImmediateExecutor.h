#pragma once

#include "ExecutorBase.h"
#include "../../disassembly/instructions/UnconditionalBranchImmediateInstruction.h"

class UnconditionalBranchImmediateExecutor : ExecutorBase {
public:
	explicit UnconditionalBranchImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(UnconditionalBranchImmediateInstruction& instruction);
};
