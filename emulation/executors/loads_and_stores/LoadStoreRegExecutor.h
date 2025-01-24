#pragma once

#include "../../../disassembly/instructions/loads_and_stores/LoadStoreReg.h"
#include "../ExecutorBaseTemplated.h"

namespace Executors::LoadsAndStores {
	class LoadStoreRegExecutor : public ExecutorBaseT<InstructionDefs::LoadsAndStores::LoadStoreReg> {
	public:
		LoadStoreRegExecutor() : ExecutorBaseT("LoadStoreRegExecutor"){}
		void execute(const InstructionDefs::LoadsAndStores::LoadStoreReg& instruction, AArch64Cpu& cpu) override;
	};
}
