#pragma once

#include "../ExecutorBase.h"
#include "../../../disassembly/instructions/loads_and_stores/LoadStoreRegUnsignedImm.h"

namespace Executors::LoadsAndStores {
	class LoadStoreRegUnsignedImm : ExecutorBase {
	public:
		explicit LoadStoreRegUnsignedImm(const std::shared_ptr<AArch64Cpu> &cpu);

		void execute(InstructionDefs::LoadsAndStores::LoadStoreRegUnsignedImm& instruction);
	};
}
