#pragma once

#include "../ExecutorBase.h"
#include "../../../disassembly/instructions/loads_and_stores/LoadStoreRegisterPair.h"
#include "../ExecutorBaseTemplated.h"

namespace Executors::LoadsAndStores {
	class LoadStoreRegPairExecutor
			: public ExecutorBaseT<InstructionDefs::LoadsAndStores::LoadStoreRegisterPair> {
	public:
		void execute(
				const InstructionDefs::LoadsAndStores::LoadStoreRegisterPair &instruction,
				AArch64Cpu& cpu) override;
	};
}
