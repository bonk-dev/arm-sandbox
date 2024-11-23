#pragma once
#include "../AArch64Cpu.h"
#include "ExecutorBase.h"
#include "../../disassembly/instructions/AddImmediate.h"

class AddSubImmediateExecutor : ExecutorBase {
public:
	explicit AddSubImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(InstructionDefs::AddImmediate& instruction) const;
};
