#include "AddSubImmediateExecutor.h"

AddSubImmediateExecutor::AddSubImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu) : ExecutorBase(cpu) {}

// TODO: Make an executor template?
void AddSubImmediateExecutor::execute(InstructionDefs::AddImmediateInstruction& instruction) const {
    const auto cpu = this->get_cpu().get();

    uint64_t val = instruction.is_64bit
        ? cpu->read_gp_register_64(instruction.source_reg_index)
        : cpu->read_gp_register_32(instruction.source_reg_index);

    if (instruction.shift_12) {
        instruction.immediate <<= 12;
    }

    if (instruction.is_subtraction) {
        val -= instruction.immediate;
    }
    else {
        val += instruction.immediate;
    }

    if (instruction.set_flags) {
        throw std::runtime_error("Set flags is not implemented");
    }

    if (instruction.is_64bit) {
        cpu->write_gp_register_64(instruction.destination_reg_index, val);
    }
    else {
        cpu->write_gp_register_32(instruction.destination_reg_index, val);
    }
}

