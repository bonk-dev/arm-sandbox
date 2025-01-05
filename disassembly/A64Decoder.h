#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "../emulation/emu_types.h"

enum class InstructionType {
	AddOrSubImmediate,
	Hint,
	ConditionalBranchImmediate,
	UnconditionalBranchImmediate,
	UnconditionalBranchRegister,
	PcRelativeAddressing,
	MoveWideImmediate,
	LogicalShiftedRegister,
	LoadStoreRegisterPair,
	LoadStoreRegisterUnsignedImm,
	ReservedCall,
	Undefined = 0xFFFFFFF
};

class A64Decoder {
private:
	uint32_t _last_raw_instruction;
public:
	InstructionType decodeNextType(uint32_t encodedInstruction);
	uint32_t getRawInstruction() const;
};