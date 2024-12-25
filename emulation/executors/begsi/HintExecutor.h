#pragma once
#include "../ExecutorBaseTemplated.h"
#include "../../../disassembly/instructions/begsi/Hint.h"

namespace Executors::Begsi {
    class HintExecutor final : public ExecutorBaseT<InstructionDefs::Begsi::Hint> {
    public:
        explicit HintExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
        void execute(const InstructionDefs::Begsi::Hint &details) override;
    };
}
