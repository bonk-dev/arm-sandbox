#include "LoadStoreRegPairExecutor.h"

LoadStoreRegPairExecutor::LoadStoreRegPairExecutor(const std::shared_ptr<AArch64Cpu> &cpu) : ExecutorBase(cpu) {}

void LoadStoreRegPairExecutor::execute(LoadStoreRegisterPairInstruction instruction) {
	throw std::runtime_error("Not implemented");
}
