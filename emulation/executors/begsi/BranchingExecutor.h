#pragma once
#include "../ExecutorBaseTemplated.h"
#include "../../BranchType.h"

namespace Executors::Begsi {
	template<class InstructionDetailsT>
	class BranchingExecutor : public ExecutorBaseT<InstructionDetailsT> {
	protected:
		void branchTo(virtual_address_t destination, [[maybe_unused]] Emulation::BranchType branchType,
					  [[maybe_unused]] bool isCondtitional, bool isLink, AArch64Cpu& cpu);
	public:
		explicit BranchingExecutor(const char* name) : ExecutorBaseT<InstructionDetailsT>(name) {}
	};

	template<class InstructionDetailsT>
	void BranchingExecutor<InstructionDetailsT>::branchTo(virtual_address_t destination,
														  [[maybe_unused]] Emulation::BranchType branchType,
														  [[maybe_unused]] bool isCondtitional, bool isLink,
														  AArch64Cpu& cpu) {
		uint64_t pc = cpu.getProgramCounter();

		if (isLink) {
			cpu.writeRegister(30, pc + 4, 64);
		}

		pc = destination;
		cpu.setProgramCounter(pc);
	}
}
