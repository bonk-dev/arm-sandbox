#pragma once

#include "../../../disassembly/instructions/data_proc_imm/FormPcRelAddress.h"
#include "../ExecutorBaseTemplated.h"

namespace Executors::DataProcImm {
	class FormPcRelAddressExecutor : public ExecutorBaseT<InstructionDefs::DataProcImm::FormPcRelAddress> {
	public:
		FormPcRelAddressExecutor(): ExecutorBaseT("FormPcRelAddressExecutor") {}
		void execute(const InstructionDefs::DataProcImm::FormPcRelAddress &instruction, AArch64Cpu& cpu) override;
	};
}
