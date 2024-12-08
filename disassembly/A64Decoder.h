#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "../emulation/emu_types.h"

enum class InstructionType {
	AddOrSubImmediate,
	UnconditionalBranchImmediate,
	PcRelativeAddressing,
	MoveWideImmediate,
	LoadStoreRegisterPair,
	LoadStoreRegisterUnsignedImm,
	Undefined = 0xFFFFFFF
};

class A64Decoder {
private:
	std::vector<std::byte> _code;
	int _index;
	uint32_t _last_raw_instruction;
public:
	explicit A64Decoder(std::vector<std::byte>& code);
	InstructionType decodeNextType();
	uint32_t getRawInstruction();

	template<class InstructionDetailsT>
	[[nodiscard]] InstructionDetailsT decode_details() const {
		return InstructionDetailsT(this->_last_raw_instruction);
	}
};