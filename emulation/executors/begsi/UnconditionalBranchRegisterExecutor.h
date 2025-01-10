#pragma once

#include "BranchingExecutor.h"
#include "../../../disassembly/instructions/begsi/UnconditionalBranchRegister.h"

namespace Executors::Begsi {
	class UnconditionalBranchRegisterExecutor
			: public BranchingExecutor<InstructionDefs::Begsi::UnconditionalBranchRegister> {
	public:
		UnconditionalBranchRegisterExecutor() : BranchingExecutor("UnconditionalBranchRegisterExecutor") {}
		void execute(const InstructionDefs::Begsi::UnconditionalBranchRegister &instruction, AArch64Cpu& cpu) override;
	};
}
