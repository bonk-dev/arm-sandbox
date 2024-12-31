#pragma once

#include "../../../disassembly/instructions/begsi/ConditionalBranchImmediate.h"
#include "BranchingExecutor.h"

namespace Executors::Begsi {
	class ConditionalBranchImmediateExecutor : public BranchingExecutor<InstructionDefs::Begsi::ConditionalBranchImmediate> {
	public:
		explicit ConditionalBranchImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
		void execute(const InstructionDefs::Begsi::ConditionalBranchImmediate& instruction) override;
	};
}
