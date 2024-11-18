#pragma once

#include "ExecutorBase.h"
#include "../../disassembly/instructions/LoadRegisterPairInstruction.h"

class LoadStoreRegPairExecutor : ExecutorBase {
private:
	uintptr_t calc_next_address(LoadStoreRegisterPairInstruction& instruction) const;
public:
	explicit LoadStoreRegPairExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(LoadStoreRegisterPairInstruction& instruction);
};
