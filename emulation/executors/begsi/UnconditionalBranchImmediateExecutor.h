#pragma once

#include "../ExecutorBase.h"
#include "../../../disassembly/instructions/begsi/UnconditionalBranchImmediate.h"
#include "BranchingExecutor.h"

namespace Executors::Begsi {
	class UnconditionalBranchImmediateExecutor : public BranchingExecutor<InstructionDefs::Begsi::UnconditionalBranchImmediate> {
	public:
		UnconditionalBranchImmediateExecutor() : BranchingExecutor("UnconditionalBranchImmediateExecutor"){}
		void execute(const InstructionDefs::Begsi::UnconditionalBranchImmediate& instruction, AArch64Cpu& cpu) override;
	};
}
