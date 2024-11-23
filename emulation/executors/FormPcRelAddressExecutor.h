#pragma once

#include "ExecutorBase.h"
#include "../../disassembly/instructions/data_proc_imm/FormPcRelAddress.h"

class FormPcRelAddressExecutor : ExecutorBase {
public:
	explicit FormPcRelAddressExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(InstructionDefs::DataProcImm::FormPcRelAddress& instruction);
};
