#include "FormPcRelAddressExecutor.h"

FormPcRelAddressExecutor::FormPcRelAddressExecutor(const std::shared_ptr<AArch64Cpu> &cpu) : ExecutorBase(cpu) {}

void FormPcRelAddressExecutor::execute(FormPcRelAddressInstruction &instruction) const {
	const auto cpu = this->get_cpu().get();
	throw std::runtime_error("Not implemented");
}


