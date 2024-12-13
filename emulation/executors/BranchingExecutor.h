#pragma once
#include "ExecutorBase'.h"
#include "../BranchType.h"

template <class InstructionDetailsT>
class BranchingExecutor : public ExecutorBaseT<InstructionDetailsT> {
protected:
	explicit BranchingExecutor(const std::shared_ptr<AArch64Cpu> &cpu): ExecutorBaseT<InstructionDetailsT>(cpu) {}
	void branchTo(virtual_address_t destination, [[maybe_unused]] Emulation::BranchType branchType, [[maybe_unused]] bool isCondtitional, bool isLink);
};

template<class InstructionDetailsT>
void BranchingExecutor<InstructionDetailsT>::branchTo(virtual_address_t destination, [[maybe_unused]] Emulation::BranchType branchType,
													  [[maybe_unused]] bool isCondtitional, bool isLink) {
	uint64_t pc = this->get_cpu()->getProgramCounter();

	if (isLink) {
		this->get_cpu()->writeGpRegister64(30, pc + 4);
	}

	pc = destination;
	this->get_cpu()->setProgramCounter(pc);
}
