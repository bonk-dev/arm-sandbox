#pragma once

#include "../../../disassembly/instructions/data_proc_imm/LogicalImmediate.h"
#include "../ExecutorBaseTemplated.h"

namespace Executors::DataProcImm {
	class LogicalImmediateExecutor : public ExecutorBaseT<InstructionDefs::DataProcImm::LogicalImmediate> {
	public:
		LogicalImmediateExecutor(): ExecutorBaseT("LogicalImmediateExecutor") {}
		void execute(const InstructionDefs::DataProcImm::LogicalImmediate &instruction, AArch64Cpu& cpu) override;
	};
}