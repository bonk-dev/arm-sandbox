#include "ReservedCallExecutor.h"
#include "../../emul_registers.h"

Executors::Reserved::ReservedCallExecutor::ReservedCallExecutor(const std::shared_ptr<AArch64Cpu> &cpu)
	: ExecutorBaseT<InstructionDefs::Reserved::ReservedCall>(cpu) {}

void Executors::Reserved::ReservedCallExecutor::execute(const InstructionDefs::Reserved::ReservedCall &instruction) {
	switch (instruction.call_type) {
		case InstructionDefs::Reserved::ReservedCalls::LibraryCall: {
			const auto symbolIndex = this->get_cpu()->readEmulRegister(Emulation::EMUL_LIB_CALL_INDEX);
			std::cout << "[ReservedCallExecutor] Calling symbol " << symbolIndex << std::endl;

			const auto libraryImplementation = this->get_cpu()->getMapper().getLibraryImplementation(symbolIndex);
			libraryImplementation();

			break;
		}
		default:
			throw std::runtime_error("Unsupported reserved call");
	}
}
