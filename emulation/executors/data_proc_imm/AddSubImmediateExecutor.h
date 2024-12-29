#pragma once
#include "../../AArch64Cpu.h"
#include "../../../disassembly/instructions/data_proc_imm/AddImmediate.h"
#include "../ExecutorBaseTemplated.h"

namespace Executors::DataProcImm {
	class AddSubImmediateExecutor : public ExecutorBaseT<InstructionDefs::DataProcImm::AddImmediate> {
	public:
		explicit AddSubImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu);

		void execute(const InstructionDefs::DataProcImm::AddImmediate &instruction) override;
	};
};
