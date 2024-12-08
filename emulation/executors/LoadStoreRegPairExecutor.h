#pragma once

#include "ExecutorBase.h"
#include "../../disassembly/instructions/loads_and_stores/LoadRegisterPair.h"

class LoadStoreRegPairExecutor : public ExecutorBase {
private:
	uintptr_t calc_next_address(InstructionDefs::LoadsAndStores::LoadStoreRegisterPairInstruction& instruction);
public:
	explicit LoadStoreRegPairExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(InstructionDefs::LoadsAndStores::LoadStoreRegisterPairInstruction& instruction);
};
