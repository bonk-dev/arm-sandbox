#include "ReservedCallExecutor.h"

Executors::Reserved::ReservedCallExecutor::ReservedCallExecutor(const std::shared_ptr<AArch64Cpu> &cpu)
	: ExecutorBaseT<InstructionDefs::Reserved::ReservedCall>(cpu) {}

void Executors::Reserved::ReservedCallExecutor::execute(const InstructionDefs::Reserved::ReservedCall &instruction) {
	throw std::runtime_error("Not implemented");
}
