#pragma once
#include "../AArch64Cpu.h"

class EmulatedSymbol {
public:
	virtual void execute(AArch64Cpu& cpu) {}
	virtual ~EmulatedSymbol() = default;
};