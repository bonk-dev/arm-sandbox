#pragma once

#include "../AArch64Cpu.h"

class ExecutorBase {
private:
	std::shared_ptr<AArch64Cpu> _cpu;
protected:
	[[nodiscard]] std::shared_ptr<AArch64Cpu>& get_cpu();
public:
	explicit ExecutorBase(const std::shared_ptr<AArch64Cpu> &cpu);
	virtual void decodeAndExecute(uint32_t encoded) {
		throw std::runtime_error("Executor not implemented");
	}
	virtual ~ExecutorBase() = default;
};
