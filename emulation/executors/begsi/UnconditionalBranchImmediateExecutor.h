#pragma once

#include "../ExecutorBase.h"
#include "../../../disassembly/instructions/begsi/UnconditionalBranchImmediate.h"
#include "../BranchingExecutor.h"

namespace Executors::Begsi {
	class UnconditionalBranchImmediateExecutor : public BranchingExecutor<InstructionDefs::Begsi::UnconditionalBranchImmediate> {
	public:
		explicit UnconditionalBranchImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
		void execute(const InstructionDefs::Begsi::UnconditionalBranchImmediate& instruction) override;
	};
}
