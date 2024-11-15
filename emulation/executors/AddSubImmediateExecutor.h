#pragma once
#include "../AArch64Cpu.h"
#include "../../disassembly/A64Decoder.h"

class AddSubImmediateExecutor {
private:
    std::shared_ptr<AArch64Cpu> _cpu;
public:
    explicit AddSubImmediateExecutor(const std::shared_ptr<AArch64Cpu> &cpu);

    void execute(AddImmediateInstruction& instruction) const;
};
