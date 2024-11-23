#include "LoadStoreRegUnsignedImm.h"
#include "../../exceptions/FeatureFpNotImplemented.h"

Executors::LoadsAndStores::LoadStoreRegUnsignedImm::LoadStoreRegUnsignedImm(const std::shared_ptr<AArch64Cpu> &cpu) : ExecutorBase(cpu) {}

void Executors::LoadsAndStores::LoadStoreRegUnsignedImm::execute(InstructionDefs::LoadsAndStores::LoadStoreRegUnsignedImm& instruction) {
	if (instruction.is_simd) {
		throw Exceptions::FeatureFpNotImplemented("LoadStoreRegUnsignedImm");
	}

	throw std::runtime_error("LDR/STR Not implemented");
}
