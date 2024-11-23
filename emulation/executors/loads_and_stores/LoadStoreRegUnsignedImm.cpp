#include "LoadStoreRegUnsignedImm.h"

Executors::LoadsAndStores::LoadStoreRegUnsignedImm::LoadStoreRegUnsignedImm(const std::shared_ptr<AArch64Cpu> &cpu) : ExecutorBase(cpu) {}

void Executors::LoadsAndStores::LoadStoreRegUnsignedImm::execute(InstructionDefs::LoadsAndStores::LoadStoreRegUnsignedImm& instruction) {
	throw std::runtime_error("LDR/STR Not implemented");
}
