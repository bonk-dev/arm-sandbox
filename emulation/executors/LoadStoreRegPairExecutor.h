#pragma once

#include "ExecutorBase.h"
#include "../../disassembly/instructions/LoadRegisterPair.h"

class LoadStoreRegPairExecutor : ExecutorBase {
private:
	uintptr_t calc_next_address(InstructionDefs::LoadStoreRegisterPairInstruction& instruction) const;
public:
	explicit LoadStoreRegPairExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(InstructionDefs::LoadStoreRegisterPairInstruction& instruction);
};
