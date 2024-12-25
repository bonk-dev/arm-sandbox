#include "LogicalShiftedRegisterExecutor.h"

Executors::DataProcReg::LogicalShiftedRegisterExecutor::LogicalShiftedRegisterExecutor(
    const std::shared_ptr<AArch64Cpu> &cpu) : ExecutorBaseT(cpu){}

void Executors::DataProcReg::LogicalShiftedRegisterExecutor::execute(
    const InstructionDefs::DataProcReg::LogicalShiftedRegister &details) {
    throw std::runtime_error("Not implemented");
}
