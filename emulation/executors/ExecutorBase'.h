#pragma once

#include "ExecutorBase.h"
#include "../../disassembly/disassembly.h"

template <class InstructionDetailsT>
class ExecutorBaseT : public ExecutorBase {
public:
	explicit ExecutorBaseT(const std::shared_ptr<AArch64Cpu> &cpu) : ExecutorBase(cpu) {}

	virtual void execute(const InstructionDetailsT& details) = 0;
	void decodeAndExecute(uint32_t encoded) override {
		const auto details = InstructionDetailsT(encoded);
		// TODO: Print conditionally
		std::cout << disassembly::to_pretty_string(details) << std::endl;
		this->execute(details);
	}
};