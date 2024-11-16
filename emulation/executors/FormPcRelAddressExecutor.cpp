#include <cmath>
#include "FormPcRelAddressExecutor.h"

FormPcRelAddressExecutor::FormPcRelAddressExecutor(const std::shared_ptr<AArch64Cpu> &cpu) : ExecutorBase(cpu) {}

void FormPcRelAddressExecutor::execute(FormPcRelAddressInstruction &instruction) const {
	const auto cpu = this->get_cpu().get();

	uint64_t pc = cpu->get_program_counter();
	uint64_t result;
	if (instruction.rel_to_4kb_page) {
		// 2^13 has the 13th bit set, 2^13 - 1 has the bottom 12 bits set. E.g.:
		// 2^13     == 1000000000000
		// 2^13 - 1 == 0111111111111
		const auto mask_12 = static_cast<uint64_t>(std::pow(2, 13) - 1);
		const uint64_t mask_not_12 = ~mask_12;
		const uint64_t base = pc & mask_not_12;

		const uint64_t shifted_imm = instruction.immediate << 12;
		result = base + shifted_imm;
	}
	else {
		result = pc + instruction.immediate;
	}

	cpu->write_gp_register_64(instruction.destination_reg_index, result);
}


