#pragma once

#include "ExecutorBase.h"
#include "../../disassembly/instructions/FormPcRelAddress.h"

class FormPcRelAddressExecutor : ExecutorBase {
public:
	explicit FormPcRelAddressExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(InstructionDefs::FormPcRelAddress& instruction) const;
};
