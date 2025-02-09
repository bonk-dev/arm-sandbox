#pragma once
#include "../ExecutorBaseTemplated.h"
#include "../../../disassembly/instructions/data_proc_simd/AdvancedSimdCopy.h"

namespace Executors::DataProcReg {
	class AdvancedSimdCopyExecutor final : public ExecutorBaseT<InstructionDefs::DataProcSimd::AdvancedSimdCopy> {
	public:
		AdvancedSimdCopyExecutor() : ExecutorBaseT("AdvancedSimdCopyExecutor") {}
		void execute(const InstructionDefs::DataProcSimd::AdvancedSimdCopy &details, AArch64Cpu& cpu) override;
	};
}
