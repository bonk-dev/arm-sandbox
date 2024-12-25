#include "HintExecutor.h"

namespace Executors::Begsi {
    HintExecutor::HintExecutor(const std::shared_ptr<AArch64Cpu> &cpu) : ExecutorBaseT(cpu) {}

    void HintExecutor::execute(const InstructionDefs::Begsi::Hint &details) {
        // do nothing
    }
}
