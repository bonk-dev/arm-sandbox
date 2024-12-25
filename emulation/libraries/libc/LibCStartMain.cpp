#include "LibCStartMain.h"

void Emulation::Libraries::LibC::LibCStartMain::execute(AArch64Cpu &cpu) {
    const virtual_address_t userEntryPointAddress = cpu.readGpRegister64(0);
    const int argc = static_cast<int>(cpu.readGpRegister64(1));
    const virtual_address_t argvPtr = cpu.readGpRegister64(2);

    // X3 == init (unused)
    // X4 == fini (unused)
    // X5 == rtld_fini (unused)
    // X7 == stack_end (unused)

    std::cout << "[__libc_start_main]: Calling main" << std::endl;

    // int main(int argc, char** argv),
    // X0 == argc
    // X1 == argv
    cpu.writeGpRegister64(0, argc);
    cpu.writeGpRegister64(1, argvPtr);

    cpu.setProgramCounter(userEntryPointAddress);
}
