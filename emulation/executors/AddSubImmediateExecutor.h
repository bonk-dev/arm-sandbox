#pragma once
#include "../AArch64Cpu.h"
#include "ExecutorBase.h"
#include "../../disassembly/instructions/AddImmediateInstruction.h"

class AddSubImmediateExecutor : ExecutorBase {
public:
	explicit AddSubImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(AddImmediateInstruction& instruction) const;
};
