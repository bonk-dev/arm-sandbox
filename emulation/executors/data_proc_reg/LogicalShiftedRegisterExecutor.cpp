#include "LogicalShiftedRegisterExecutor.h"

#include <limits>

void Executors::DataProcReg::LogicalShiftedRegisterExecutor::execute(
    const InstructionDefs::DataProcReg::LogicalShiftedRegister &details, AArch64Cpu& cpu) {
    if (!details.is64Bit && details.shiftAmount >= 32) {
        throw std::runtime_error("32-bit logical shifted reg. optional shift amount needs to be in the 0-31 range");
    }

    uint64_t first = cpu.readRegister(details.operand1Reg, 64);
    uint64_t second = cpu.readRegister(details.operand2Reg, 64);

    if (!details.is64Bit) {
        first &= std::numeric_limits<uint32_t>::max();
        second &= std::numeric_limits<uint32_t>::max();
    }

    if (details.shiftAmount > 0) {
        switch (details.shiftType) {
            case InstructionDefs::DataProcReg::LogicalShiftedRegister::ShiftType::LogicalShiftLeft:
                second <<= details.shiftAmount;
            break;
            case InstructionDefs::DataProcReg::LogicalShiftedRegister::ShiftType::LogicalShiftRight:
                second >>= details.shiftAmount;
            break;
            case InstructionDefs::DataProcReg::LogicalShiftedRegister::ShiftType::ArythmeticShiftRight: {
                auto secondSigned = static_cast<int64_t>(second);
                secondSigned >>= details.shiftAmount;
                second = static_cast<uint64_t>(secondSigned);
                break;
            }
            case InstructionDefs::DataProcReg::LogicalShiftedRegister::ShiftType::RotateRight: {
                if (details.is64Bit) {
                    second = std::rotr(second, details.shiftAmount);
                }
                else {
                    const auto second32 = static_cast<uint32_t>(second);
                    second = std::rotr(second32, details.shiftAmount);
                }
                break;
            }
            default:
                throw std::runtime_error("Invalid shift type");
        }
    }

    if (details.negate) {
        second = ~second;
    }

    uint64_t result;
    switch (details.operation) {
        case InstructionDefs::LogicalOperation::And:
            result = first & second;
        break;
        case InstructionDefs::LogicalOperation::Or:
            result = first | second;
        break;
        case InstructionDefs::LogicalOperation::Xor:
            result = first ^ second;
        break;
        case InstructionDefs::LogicalOperation::AndSetFlags:
            throw std::runtime_error("Set flags not implemented");
        default:
            throw std::runtime_error("Invalid operation");
    }

	cpu.writeRegister(details.destinationReg, result, details.is64Bit ? 64 : 32);
}
