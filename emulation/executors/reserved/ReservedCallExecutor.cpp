#include "ReservedCallExecutor.h"
#include "../../emul_registers.h"

Executors::Reserved::ReservedCallExecutor::ReservedCallExecutor(const std::shared_ptr<AArch64Cpu> &cpu)
	: ExecutorBaseT<InstructionDefs::Reserved::ReservedCall>(cpu) {}

void Executors::Reserved::ReservedCallExecutor::execute(const InstructionDefs::Reserved::ReservedCall &instruction) {
	switch (instruction.call_type) {
		case InstructionDefs::Reserved::ReservedCalls::LibraryCall: {
			std::cout << "[ReservedCallExecutor] Calling symbol " << instruction.immediate << std::endl;

			auto cpu = this->get_cpu().get();
			if (!cpu->getMapper().hasLibraryImplementation(instruction.immediate)) {
				std::cerr << "[ReservedCallExecutor] No library implementation was found for symbol #" << instruction.immediate << std::endl;
				throw std::runtime_error("No lib implementation was found");
			}
			else {
				auto& impl = cpu->getMapper().getLibraryImplementation(instruction.immediate);
				impl.execute();
			}

			break;
		}
		default:
			throw std::runtime_error("Unsupported reserved call");
	}
}
