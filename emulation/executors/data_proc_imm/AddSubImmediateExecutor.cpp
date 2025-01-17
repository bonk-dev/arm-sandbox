#include "AddSubImmediateExecutor.h"
#include "../../nzcv.h"

namespace {
	template<typename IntType>
	IntType add_with_carry(IntType a, IntType b, bool carry) {
		uint64_t discarded_nzcv = 0;
		return add_with_carry(a, b, carry, discarded_nzcv);
	}

	template<typename UnsignedIntType, typename SignedIntType>
	UnsignedIntType add_with_carry(UnsignedIntType a, UnsignedIntType b, bool carry, uint64_t& nzcv) {
		const UnsignedIntType u_sum = a + b + static_cast<unsigned int>(carry);
		const UnsignedIntType s_sum = static_cast<SignedIntType>(a) + static_cast<SignedIntType>(b) + static_cast<unsigned int>(carry);

		nzcv = nzcv::negative(nzcv, s_sum < 0);
		nzcv = nzcv::zero(nzcv, u_sum == 0);
		nzcv = nzcv::carry(nzcv, u_sum < a);
		nzcv = nzcv::overflow(nzcv, s_sum < static_cast<SignedIntType>(a));

		return u_sum;
	}
}

void Executors::DataProcImm::AddSubImmediateExecutor::execute(
		const InstructionDefs::DataProcImm::AddImmediate& instruction, AArch64Cpu& cpu) {
    uint64_t val = instruction.is_64bit
        ? cpu.readGpRegister64(instruction.source_reg_index, true)
        : cpu.readGpRegister32(instruction.source_reg_index, true);

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
		result = add_with_carry<uint64_t, int64_t>(val, imm, false, nzcv);
	}
	else {
		result = add_with_carry<uint32_t, int32_t>(val, imm, false, nzcv);
	}

    if (instruction.set_flags) {
		cpu.writeNzcvRegsiter(nzcv);
    }

	// SP is not writeable using ADDS/SUBS
	// it's used as the destination then the result is to be discarded, for example when comparing values to 0 (CMP Xn, #0)
	if (instruction.is_64bit) {
		cpu.writeGpRegister64(instruction.destination_reg_index, result, !instruction.set_flags);
	}
	else {
		cpu.writeGpRegister32(instruction.destination_reg_index, result, !instruction.set_flags);
	}
}
