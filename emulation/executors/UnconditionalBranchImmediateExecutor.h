#pragma once

#include "ExecutorBase.h"
#include "../../disassembly/instructions/begsi/UnconditionalBranchImmediate.h"
#include "ExecutorBase'.h"

class UnconditionalBranchImmediateExecutor : public ExecutorBaseT<InstructionDefs::Begsi::UnconditionalBranchImmediate> {
public:
	explicit UnconditionalBranchImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(const InstructionDefs::Begsi::UnconditionalBranchImmediate& instruction) override;
};
