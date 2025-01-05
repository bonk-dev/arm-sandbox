#pragma once

#include "../../../disassembly/instructions/data_proc_imm/MoveWideImmediate.h"
#include "../ExecutorBaseTemplated.h"

namespace Executors::DataProcImm {
	class MoveWideImmediateExecutor : public ExecutorBaseT<InstructionDefs::DataProcImm::MoveWideImmediate> {
	public:
		void execute(const InstructionDefs::DataProcImm::MoveWideImmediate &instruction, AArch64Cpu& cpu) override;
	};
}
