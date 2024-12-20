#pragma once
#include <memory>

#include "../AArch64Cpu.h"

namespace Emulation::Libraries {
    class DummyNamePrinter {
        DummyNamePrinter(const char* symbolName, const std::shared_ptr<AArch64Cpu>& cpu);
        void execute();
    };
}
