#include "ExecutorBase.h"

ExecutorBase::ExecutorBase(const std::shared_ptr<AArch64Cpu> &cpu) {
	this->_cpu = cpu;
}

std::shared_ptr<AArch64Cpu>& ExecutorBase::get_cpu() {
	return this->_cpu;
}
