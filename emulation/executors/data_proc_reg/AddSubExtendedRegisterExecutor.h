#pragma once
#include "../ExecutorBaseTemplated.h"
#include "../../../disassembly/instructions/data_proc_register/AddSubExtendedRegister.h"

namespace Executors::DataProcReg {
	class AddSubExtendedRegisterExecutor final : public ExecutorBaseT<InstructionDefs::DataProcReg::AddSubExtendedRegister> {
	public:
		AddSubExtendedRegisterExecutor() : ExecutorBaseT("AddSubExtendedRegisterExecutor") {}
		void execute(const InstructionDefs::DataProcReg::AddSubExtendedRegister &details, AArch64Cpu& cpu) override;
	};
}
