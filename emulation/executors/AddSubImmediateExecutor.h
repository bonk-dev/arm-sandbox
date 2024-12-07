#pragma once
#include "../AArch64Cpu.h"
#include "ExecutorBase.h"
#include "../../disassembly/instructions/data_proc_imm/AddImmediate.h"

class AddSubImmediateExecutor : public ExecutorBase {
public:
	explicit AddSubImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(InstructionDefs::DataProcImm::AddImmediate& instruction);
	void decodeAndExecute(uint32_t encoded) override;
};
