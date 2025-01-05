#pragma once
#include "../ExecutorBaseTemplated.h"
#include "../../../disassembly/instructions/begsi/Hint.h"

namespace Executors::Begsi {
    class HintExecutor final : public ExecutorBaseT<InstructionDefs::Begsi::Hint> {
    public:
        void execute(const InstructionDefs::Begsi::Hint &details, AArch64Cpu& cpu) override;
    };
}
