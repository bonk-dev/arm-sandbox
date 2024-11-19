#pragma once

#include "ExecutorBase.h"
#include "../../disassembly/instructions/MoveWideImmediateInstruction.h"

class MoveWideImmediateExecutor : ExecutorBase {
public:
	explicit MoveWideImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(MoveWideImmediateInstruction& instruction);
};
