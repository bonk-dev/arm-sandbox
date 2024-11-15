#pragma once
#include "../AArch64Cpu.h"
#include "../../disassembly/A64Decoder.h"
#include "ExecutorBase.h"

class AddSubImmediateExecutor : ExecutorBase {
public:
	explicit AddSubImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(AddImmediateInstruction& instruction) const;
};
