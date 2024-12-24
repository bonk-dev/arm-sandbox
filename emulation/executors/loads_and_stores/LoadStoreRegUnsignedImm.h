#pragma once

#include "../../../disassembly/instructions/loads_and_stores/LoadStoreRegUnsignedImm.h"
#include "../ExecutorBaseTemplated.h"

namespace Executors::LoadsAndStores {
	class LoadStoreRegUnsignedImm : public ExecutorBaseT<InstructionDefs::LoadsAndStores::LoadStoreRegUnsignedImm> {
	public:
		explicit LoadStoreRegUnsignedImm(const std::shared_ptr<AArch64Cpu> &cpu);
		void execute(const InstructionDefs::LoadsAndStores::LoadStoreRegUnsignedImm& instruction) override;
	};
}
