#pragma once
#include "../EmulatedSymbol.h"

namespace Emulation::Libraries::LibC {
    /**
     * @brief Implements int puts(char* s)
     */
    class Puts final : public EmulatedSymbol {
    public:
        Puts() : EmulatedSymbol("Puts") {}
        void execute(AArch64Cpu &cpu) override;
    };
}
