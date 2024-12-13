#pragma once

#include "BranchingExecutor.h"
#include "../../disassembly/instructions/begsi/UnconditionalBranchRegister.h"

class UnconditionalBranchRegisterExecutor : public BranchingExecutor<InstructionDefs::Begsi::UnconditionalBranchRegister> {
public:
	explicit UnconditionalBranchRegisterExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(const InstructionDefs::Begsi::UnconditionalBranchRegister& instruction) override;
};
