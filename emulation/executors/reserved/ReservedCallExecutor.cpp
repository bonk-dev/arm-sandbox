#include "ReservedCallExecutor.h"

Executors::Reserved::ReservedCallExecutor::ReservedCallExecutor(const std::shared_ptr<AArch64Cpu> &cpu)
	: ExecutorBaseT<InstructionDefs::Reserved::ReservedCall>(cpu) {}

Executors::Reserved::ReservedCallExecutor::ReservedCallExecutor(const std::shared_ptr<AArch64Cpu> &cpu,
                                                                const std::shared_ptr<Emulation::Libraries::Mapper> &mapper)
	: ExecutorBaseT<InstructionDefs::Reserved::ReservedCall>(cpu),
	_mapper(mapper) {}

void Executors::Reserved::ReservedCallExecutor::execute(const InstructionDefs::Reserved::ReservedCall &instruction) {
	switch (instruction.call_type) {
		case InstructionDefs::Reserved::ReservedCalls::Exit: {
			if (instruction.immediate == 0) {
				std::cout << "[ReservedCallExecutor] Clean exit" << std::endl;
				const int mainStatusCode = static_cast<int>(this->get_cpu()->readGpRegister32(0));
				this->get_cpu()->haltExecution(mainStatusCode);
			}
			break;
		}
		case InstructionDefs::Reserved::ReservedCalls::LibraryCall: {
			std::cout << "[ReservedCallExecutor] Calling symbol " << instruction.immediate << std::endl;

			if (!_mapper->hasLibraryImplementation(instruction.immediate)) {
				std::cerr << "[ReservedCallExecutor] No library implementation was found for symbol #" << instruction.immediate << std::endl;
				throw std::runtime_error("No lib implementation was found");
			}
			else {
				auto& impl = _mapper->getLibraryImplementation(instruction.immediate);
				impl.execute(*this->get_cpu());
			}

			break;
		}
		default:
			throw std::runtime_error("Unsupported reserved call");
	}
}
