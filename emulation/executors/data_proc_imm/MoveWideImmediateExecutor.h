#pragma once

#include "../../../disassembly/instructions/data_proc_imm/MoveWideImmediate.h"
#include "../ExecutorBaseTemplated.h"

namespace Executors::DataProcImm {
	class MoveWideImmediateExecutor : public ExecutorBaseT<InstructionDefs::DataProcImm::MoveWideImmediate> {
	public:
		explicit MoveWideImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu);

		void execute(const InstructionDefs::DataProcImm::MoveWideImmediate &instruction) override;
	};
}
