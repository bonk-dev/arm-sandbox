#pragma once

#include "../AArch64Cpu.h"

class ExecutorBase {
private:
	std::shared_ptr<AArch64Cpu> _cpu;
protected:
	[[nodiscard]] std::shared_ptr<AArch64Cpu>& get_cpu();
public:
	explicit ExecutorBase(const std::shared_ptr<AArch64Cpu> &cpu);
};
