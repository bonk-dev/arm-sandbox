#pragma once

#include "../ExecutorBase.h"
#include "../../../disassembly/instructions/loads_and_stores/LoadRegisterPair.h"
#include "../ExecutorBaseTemplated.h"

namespace Executors::LoadsAndStores {
	class LoadStoreRegPairExecutor
			: public ExecutorBaseT<InstructionDefs::LoadsAndStores::LoadRegisterPair> {
	private:
		uintptr_t calc_next_address(InstructionDefs::LoadsAndStores::LoadRegisterPair &instruction);
	public:
		void execute(
				const InstructionDefs::LoadsAndStores::LoadRegisterPair &instruction,
				AArch64Cpu& cpu) override;
	};
}
