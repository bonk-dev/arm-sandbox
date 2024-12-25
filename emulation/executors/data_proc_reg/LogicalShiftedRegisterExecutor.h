#pragma once
#include "../ExecutorBaseTemplated.h"
#include "../../../disassembly/instructions/data_proc_register/LogicalShiftedRegister.h"

namespace Executors::DataProcReg {
    class LogicalShiftedRegisterExecutor final : public ExecutorBaseT<InstructionDefs::DataProcReg::LogicalShiftedRegister> {
    public:
        explicit LogicalShiftedRegisterExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
        void execute(const InstructionDefs::DataProcReg::LogicalShiftedRegister &details) override;
    };
}
