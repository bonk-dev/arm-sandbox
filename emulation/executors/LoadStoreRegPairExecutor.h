#pragma once

#include "ExecutorBase.h"
#include "../../disassembly/A64Decoder.h"

class LoadStoreRegPairExecutor : ExecutorBase {
public:
	explicit LoadStoreRegPairExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
	void execute(LoadStoreRegisterPairInstruction instruction);
};
