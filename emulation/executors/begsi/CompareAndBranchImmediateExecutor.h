#pragma once

#include "../../../disassembly/instructions/begsi/CompareAndBranchImmediate.h"
#include "BranchingExecutor.h"

namespace Executors::Begsi {
	class CompareAndBranchImmediateExecutor : public BranchingExecutor<InstructionDefs::Begsi::CompareAndBranchImmediate> {
	public:
		CompareAndBranchImmediateExecutor() : BranchingExecutor("CompareAndBranchImmediateExecutor") {}
		void execute(const InstructionDefs::Begsi::CompareAndBranchImmediate& instruction, AArch64Cpu& cpu) override;
	};
}