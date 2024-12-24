#pragma once

#include "../../disassembly/instructions/data_proc_imm/FormPcRelAddress.h"
#include "ExecutorBaseTemplated.h"

class FormPcRelAddressExecutor : public ExecutorBaseT<InstructionDefs::DataProcImm::FormPcRelAddress> {
public:
	explicit FormPcRelAddressExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(const InstructionDefs::DataProcImm::FormPcRelAddress& instruction) override;
};
