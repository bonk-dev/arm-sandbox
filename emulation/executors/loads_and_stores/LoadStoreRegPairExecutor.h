#pragma once

#include "../ExecutorBase.h"
#include "../../../disassembly/instructions/loads_and_stores/LoadStoreRegisterPair.h"
#include "../ExecutorBaseTemplated.h"

namespace Executors::LoadsAndStores {
	class LoadStoreRegPairExecutor
			: public ExecutorBaseT<InstructionDefs::LoadsAndStores::LoadStoreRegisterPair> {
	private:
		uintptr_t calc_next_address(InstructionDefs::LoadsAndStores::LoadStoreRegisterPair &instruction);
	public:
		void execute(
				const InstructionDefs::LoadsAndStores::LoadStoreRegisterPair &instruction,
				AArch64Cpu& cpu) override;
	};
}
