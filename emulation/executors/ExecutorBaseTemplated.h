#pragma once

#include "ExecutorBase.h"
#include "../../disassembly/disassembly.h"

template <class InstructionDetailsT>
class ExecutorBaseT : public ExecutorBase {
public:
	virtual void execute(const InstructionDetailsT& details, AArch64Cpu& cpu) = 0;
	void decodeAndExecute(uint32_t encoded, AArch64Cpu& cpu) override {
		const auto details = InstructionDetailsT(encoded);
		// TODO: Print conditionally
		std::cout << disassembly::to_pretty_string(details) << std::endl;
		this->execute(details, cpu);
	}
};