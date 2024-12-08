#pragma once

#include "ExecutorBase.h"
#include "../../disassembly/instructions/loads_and_stores/LoadRegisterPair.h"
#include "ExecutorBase'.h"

class LoadStoreRegPairExecutor : public ExecutorBaseT<InstructionDefs::LoadsAndStores::LoadStoreRegisterPairInstruction> {
private:
	uintptr_t calc_next_address(InstructionDefs::LoadsAndStores::LoadStoreRegisterPairInstruction& instruction);
public:
	explicit LoadStoreRegPairExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(const InstructionDefs::LoadsAndStores::LoadStoreRegisterPairInstruction& instruction) override;
};
