#pragma once

#include "../../../disassembly/instructions/begsi/ConditionalBranchImmediate.h"
#include "BranchingExecutor.h"

namespace Executors::Begsi {
	class ConditionalBranchImmediateExecutor : public BranchingExecutor<InstructionDefs::Begsi::ConditionalBranchImmediate> {
	public:
		ConditionalBranchImmediateExecutor() : BranchingExecutor("ConditionalBranchImmediateExecutor") {}
		void execute(const InstructionDefs::Begsi::ConditionalBranchImmediate& instruction, AArch64Cpu& cpu) override;
	};
}
