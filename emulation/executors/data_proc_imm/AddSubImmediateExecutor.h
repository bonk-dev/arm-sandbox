#pragma once
#include "../../AArch64Cpu.h"
#include "../../../disassembly/instructions/data_proc_imm/AddImmediate.h"
#include "../ExecutorBaseTemplated.h"

namespace Executors::DataProcImm {
	class AddSubImmediateExecutor : public ExecutorBaseT<InstructionDefs::DataProcImm::AddImmediate> {
	public:
		void execute(const InstructionDefs::DataProcImm::AddImmediate &instruction, AArch64Cpu& cpu) override;
	};
}
