#include "LibCStartMain.h"

void Emulation::Libraries::LibC::LibCStartMain::execute(AArch64Cpu &cpu) {
    const virtual_address_t userEntryPointAddress = cpu.readRegister(0, 64);
    const int argc = static_cast<int>(cpu.readRegister(1, 64));
    const virtual_address_t argvPtr = cpu.readRegister(2, 64);

    // X3 == init (unused)
    // X4 == fini (unused)
    // X5 == rtld_fini (unused)
    // X7 == stack_end (unused)

    _logger->info() << "Calling main" << std::endl;

    // int main(int argc, char** argv),
    // X0 == argc
    // X1 == argv
	cpu.writeRegister(0, argc, 64);
	cpu.writeRegister(1, argvPtr, 64);

    cpu.setProgramCounter(userEntryPointAddress);

    // this is a hack to jump to the clean address, because at this point LR is set to the instruction after __libc_start_main
    // which is abort().
	cpu.writeRegister(30, cpu.getCleanExitAddress(), 64);
}
