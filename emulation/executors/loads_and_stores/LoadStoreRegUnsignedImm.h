#pragma once

#include "../../../disassembly/instructions/loads_and_stores/LoadStoreRegUnsignedImm.h"
#include "../ExecutorBaseTemplated.h"

namespace Executors::LoadsAndStores {
	class LoadStoreRegUnsignedImm : public ExecutorBaseT<InstructionDefs::LoadsAndStores::LoadStoreRegUnsignedImm> {
	public:
		void execute(const InstructionDefs::LoadsAndStores::LoadStoreRegUnsignedImm& instruction, AArch64Cpu& cpu) override;
	};
}
