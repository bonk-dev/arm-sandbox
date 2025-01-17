#include "LogicalImmediateExecutor.h"
#include "../../nzcv.h"

namespace {
	bool is_negative(const uint64_t val, const bool is64Bit) {
		const unsigned int bit = is64Bit
				? 63
				: 31;
		return (val >> bit) & 1;
	}
}

void Executors::DataProcImm::LogicalImmediateExecutor::execute(
		const InstructionDefs::DataProcImm::LogicalImmediate &instruction, AArch64Cpu &cpu) {
	uint64_t val = cpu.readRegister64(instruction.sourceReg);
	switch (instruction.operation) {
		case InstructionDefs::LogicalOperation::And:
			val &= instruction.bitmask;
			break;
		case InstructionDefs::LogicalOperation::Or:
			val |= instruction.bitmask;
			break;
		case InstructionDefs::LogicalOperation::Xor:
			val ^= instruction.bitmask;
			break;
		case InstructionDefs::LogicalOperation::AndSetFlags:
			val &= instruction.bitmask;
			cpu.writeNzcvRegsiter(
					nzcv::nzcv(is_negative(val, instruction.is64Bit),
							   val == 0,
							   false,
							   false));
			break;
	}

	cpu.writeRegister64(instruction.destinationReg, val, true);
}
