#pragma once

#include "ExecutorBase.h"
#include "../../disassembly/instructions/data_proc_imm/MoveWideImmediate.h"

class MoveWideImmediateExecutor : public ExecutorBase {
public:
	explicit MoveWideImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(InstructionDefs::DataProcImm::MoveWideImmediate& instruction);
};
