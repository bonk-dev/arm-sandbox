#pragma once

#include "ExecutorBase.h"
#include "../../disassembly/instructions/begsi/UnconditionalBranchImmediate.h"

class UnconditionalBranchImmediateExecutor : public ExecutorBase {
public:
	explicit UnconditionalBranchImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(InstructionDefs::Begsi::UnconditionalBranchImmediate& instruction);
};
