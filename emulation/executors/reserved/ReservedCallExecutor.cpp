#include "ReservedCallExecutor.h"
#include "../../emul_registers.h"

Executors::Reserved::ReservedCallExecutor::ReservedCallExecutor(const std::shared_ptr<AArch64Cpu> &cpu)
	: ExecutorBaseT<InstructionDefs::Reserved::ReservedCall>(cpu) {}

void Executors::Reserved::ReservedCallExecutor::execute(const InstructionDefs::Reserved::ReservedCall &instruction) {
	switch (instruction.call_type) {
		case InstructionDefs::Reserved::ReservedCalls::LibraryCall: {
			std::cout << "[ReservedCallExecutor] Calling symbol " << instruction.immediate << std::endl;

			const auto libraryImplementation = this->get_cpu()->getMapper().getLibraryImplementation(instruction.immediate);
			if (libraryImplementation == nullptr) {
				std::cerr << "[ReservedCallExecutor] No library implementation was found for symbol #" << instruction.immediate << std::endl;
				throw std::runtime_error("No lib implementation was found");
			}
			else {
				libraryImplementation();
			}

			break;
		}
		default:
			throw std::runtime_error("Unsupported reserved call");
	}
}
