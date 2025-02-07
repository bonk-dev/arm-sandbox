#include "AddSubExtendedRegisterExecutor.h"
#include "../../carry_arithmetic.h"

namespace {
	using AddSubExtendedRegister = InstructionDefs::DataProcReg::AddSubExtendedRegister;

	uint64_t extend(uint64_t regValue, AddSubExtendedRegister::ExtendVariant variant, bool is64Bit) {
		uint64_t extended;
		switch (variant) {
			case AddSubExtendedRegister::ExtendVariant::Uxtb:
				extended = static_cast<uint8_t>(regValue);
				break;
			case AddSubExtendedRegister::ExtendVariant::Uxth:
				extended = static_cast<uint16_t>(regValue);
				break;
			case AddSubExtendedRegister::ExtendVariant::Uxtw:
				extended = static_cast<uint32_t>(regValue);
				break;
			case AddSubExtendedRegister::ExtendVariant::Uxtx:
				if (is64Bit) {
					extended = regValue;
				} else {
					extended = static_cast<uint32_t>(regValue);
				}
				break;
			case AddSubExtendedRegister::ExtendVariant::Sxtb:
				extended = static_cast<int8_t>(regValue);
				break;
			case AddSubExtendedRegister::ExtendVariant::Sxth:
				extended = static_cast<int16_t>(regValue);
				break;
			case AddSubExtendedRegister::ExtendVariant::Sxtw:
				extended = static_cast<int32_t>(regValue);
				break;
			case AddSubExtendedRegister::ExtendVariant::Sxtx:
				if (is64Bit) {
					extended = static_cast<int64_t>(regValue);;
				} else {
					extended = static_cast<int32_t>(regValue);
				}
				break;
			default:
				throw std::runtime_error("Invalid extend variant");
		}

		return extended;
	}
}

void Executors::DataProcReg::AddSubExtendedRegisterExecutor::execute(
		const AddSubExtendedRegister &details, AArch64Cpu &cpu) {
	const uint64_t firstOperand = cpu.readRegisterSp(details.firstSourceReg, details.is64Bit ? 64 : 32);
	const size_t secondOpSize = details.is64Bit
			&& (details.extendVariant == AddSubExtendedRegister::ExtendVariant::Sxtw || details.extendVariant == AddSubExtendedRegister::ExtendVariant::Uxtx)
		    ? 64
		    : 32; // no sp
	uint64_t secondOperand = cpu.readRegister(details.secondSourceReg, secondOpSize);
	secondOperand = extend(secondOperand, details.extendVariant, details.is64Bit);
	secondOperand <<= details.shiftAmount;

	if (details.subtract) {
		// U2
		secondOperand = ~(secondOperand) + 1;
	}

	uint64_t nzcv;
	if (details.is64Bit) {
		auto result = Emulation::add_with_carry<uint64_t, int64_t>(firstOperand, secondOperand, false, nzcv);
		cpu.writeRegister(details.destinationReg, result, 64);
	}
	else {
		auto result = Emulation::add_with_carry<uint32_t, int32_t>(firstOperand, secondOperand, false, nzcv);
		cpu.writeRegister(details.destinationReg, result, 64);
	}

	if (details.setFlags) {
		cpu.writeNzcvRegsiter(nzcv);
	}
}
