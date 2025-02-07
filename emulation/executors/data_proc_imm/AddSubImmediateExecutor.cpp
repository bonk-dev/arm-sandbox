#include "AddSubImmediateExecutor.h"
#include "../../nzcv.h"
#include "../../carry_arithmetic.h"

void Executors::DataProcImm::AddSubImmediateExecutor::execute(
		const InstructionDefs::DataProcImm::AddImmediate& instruction, AArch64Cpu& cpu) {
    uint64_t val = cpu.readRegisterSp(instruction.source_reg_index, instruction.is_64bit ? 64 : 32);
	uint16_t imm = instruction.immediate;
    if (instruction.shift_12) {
        imm <<= 12;
    }

	uint64_t result;
	if (instruction.is_subtraction) {
		// U2
		imm = ~(imm) + 1;
	}

	uint64_t nzcv = 0;
	if (instruction.is_64bit) {
		result = Emulation::add_with_carry<uint64_t, int64_t>(val, imm, false, nzcv);
	}
	else {
		result = Emulation::add_with_carry<uint32_t, int32_t>(val, imm, false, nzcv);
	}

    if (instruction.set_flags) {
		cpu.writeNzcvRegsiter(nzcv);
    }

	// SP is not writeable using ADDS/SUBS
	// it's used as the destination then the result is to be discarded, for example when comparing values to 0 (CMP Xn, #0)
	if (instruction.set_flags) {
		cpu.writeRegister(instruction.destination_reg_index, result, instruction.is_64bit ? 64 : 32);
	}
	else {
		cpu.writeRegisterSp(instruction.destination_reg_index, result, instruction.is_64bit ? 64 : 32);
	}
}
