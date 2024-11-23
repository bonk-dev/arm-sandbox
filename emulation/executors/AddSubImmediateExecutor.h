#pragma once
#include "../AArch64Cpu.h"
#include "ExecutorBase.h"
#include "../../disassembly/instructions/data_proc_imm/AddImmediate.h"

class AddSubImmediateExecutor : ExecutorBase {
public:
	explicit AddSubImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(InstructionDefs::DataProcImm::AddImmediate& instruction) const;
};
