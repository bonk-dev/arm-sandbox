#pragma once
#include "../ExecutorBaseTemplated.h"
#include "../../../disassembly/instructions/data_proc_register/LogicalShiftedRegister.h"

namespace Executors::DataProcReg {
    class LogicalShiftedRegisterExecutor final : public ExecutorBaseT<InstructionDefs::DataProcReg::LogicalShiftedRegister> {
    public:
        LogicalShiftedRegisterExecutor() : ExecutorBaseT("LogicalShiftedRegisterExecutor") {}
        void execute(const InstructionDefs::DataProcReg::LogicalShiftedRegister &details, AArch64Cpu& cpu) override;
    };
}
