#pragma once
#include "../../AArch64Cpu.h"
#include "../EmulatedSymbol.h"

namespace Emulation::Libraries::LibC {
    /**
     * @brief Implements int __libc_start_main(int (*main) (int, char * *, char * *), int argc, char * * ubp_av, void (*init) (void), void (*fini) (void), void (*rtld_fini) (void), void (* stack_end));
     * @see https://refspecs.linuxbase.org/LSB_3.1.1/LSB-Core-generic/LSB-Core-generic/baselib---libc-start-main-.html
     */
    class LibCStartMain final : public EmulatedSymbol {
    public:
        LibCStartMain() : EmulatedSymbol("__libc_start_main") {}
        void execute(AArch64Cpu &cpu) override;
    };
}
