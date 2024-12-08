#pragma once

#include "ExecutorBase.h"

template <class InstructionDetailsT>
class ExecutorBaseT : public ExecutorBase {
public:
	explicit ExecutorBaseT(const std::shared_ptr<AArch64Cpu> &cpu) : ExecutorBase(cpu) {}

	virtual void execute(const InstructionDetailsT& details) = 0;
	void decodeAndExecute(uint32_t encoded) override {
		const auto details = InstructionDetailsT(encoded);
		this->execute(details);
	}
};