#include "Puts.h"

void Emulation::Libraries::LibC::Puts::execute(AArch64Cpu &cpu) {
    const virtual_address_t charPtr = cpu.readGpRegister64(0);
    const std::string str = cpu.getMemory().readCString(charPtr);
    const auto result = puts(str.c_str());

    // return value is written to X0
    cpu.writeGpRegister64(0, result);
}
