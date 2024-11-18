#pragma once

#include "ExecutorBase.h"
#include "../../disassembly/instructions/FormPcRelAddressInstruction.h"

class FormPcRelAddressExecutor : ExecutorBase {
public:
	explicit FormPcRelAddressExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(FormPcRelAddressInstruction& instruction) const;
};
