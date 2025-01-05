#pragma once

#include "../ExecutorBase.h"
#include "../../../disassembly/instructions/loads_and_stores/LoadRegisterPair.h"
#include "../ExecutorBaseTemplated.h"

namespace Executors::LoadsAndStores {
	class LoadStoreRegPairExecutor
			: public ExecutorBaseT<InstructionDefs::LoadsAndStores::LoadStoreRegisterPairInstruction> {
	private:
		uintptr_t calc_next_address(InstructionDefs::LoadsAndStores::LoadStoreRegisterPairInstruction &instruction);
	public:
		void execute(
				const InstructionDefs::LoadsAndStores::LoadStoreRegisterPairInstruction &instruction,
				AArch64Cpu& cpu) override;
	};
}
