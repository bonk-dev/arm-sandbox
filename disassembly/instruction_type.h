#pragma once

enum class InstructionType {
	AddOrSubImmediate,
	Hint,
	ConditionalBranchImmediate,
	UnconditionalBranchImmediate,
	UnconditionalBranchRegister,
	CompareAndBranchImmediate,
	PcRelativeAddressing,
	LogicalImmediate,
	MoveWideImmediate,
	AddSubExtendedRegister,
	LogicalShiftedRegister,
	AdvancedSimdCopy,
	LoadStoreRegisterPair,
	LoadStoreRegister,
	LoadStoreRegisterUnsignedImm,
	ReservedCall,
	Undefined = 0xFFFFFFF
};