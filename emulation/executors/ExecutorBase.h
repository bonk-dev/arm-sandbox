#pragma once

#include "../AArch64Cpu.h"

class ExecutorBase {
public:
	virtual void decodeAndLog(uint32_t encoded) {
		throw std::runtime_error("Executor not implemented");
	}
	virtual void decodeAndExecute(uint32_t encoded, AArch64Cpu& cpu) {
		throw std::runtime_error("Executor not implemented");
	}
	virtual ~ExecutorBase() = default;
};
