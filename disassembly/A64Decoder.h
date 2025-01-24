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
	LogicalImmediate,
	MoveWideImmediate,
	AddSubExtendedRegister,
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
	[[nodiscard]] uint32_t getRawInstruction() const;
};