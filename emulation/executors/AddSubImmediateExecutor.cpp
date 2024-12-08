#include "AddSubImmediateExecutor.h"

AddSubImmediateExecutor::AddSubImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu)
	: ExecutorBaseT<InstructionDefs::DataProcImm::AddImmediate>(cpu) {}

void AddSubImmediateExecutor::execute(const InstructionDefs::DataProcImm::AddImmediate& instruction) {
    const auto cpu = this->get_cpu().get();

    uint64_t val = instruction.is_64bit
        ? cpu->readGpRegister64(instruction.source_reg_index)
        : cpu->readGpRegister32(instruction.source_reg_index);

	uint16_t imm = instruction.immediate;
    if (instruction.shift_12) {
        imm <<= 12;
    }

    if (instruction.is_subtraction) {
        val -= imm;
    }
    else {
        val += imm;
    }

    if (instruction.set_flags) {
        throw std::runtime_error("Set flags is not implemented");
    }

    if (instruction.is_64bit) {
		cpu->writeGpRegister64(instruction.destination_reg_index, val);
    }
    else {
		cpu->writeGpRegister32(instruction.destination_reg_index, val);
    }
}
