#pragma once
#include "../EmulatedSymbol.h"

namespace Emulation::Libraries::LibC {
    /**
     * @brief Implements int puts(char* s)
     */
    class Puts final : public EmulatedSymbol {
        void execute(AArch64Cpu &cpu) override;
    };
}
