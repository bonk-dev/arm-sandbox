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
	Undefined = 0xFFFFFFF
};

class A64Decoder {
private:
	std::vector<std::byte> _code;
	int _index;
	uint32_t _last_raw_instruction;
public:
	explicit A64Decoder(std::vector<std::byte>& code);
	InstructionType decode_next();

	template<class InstructionDetailsT>
	[[nodiscard]] InstructionDetailsT decode_details() const {
		return InstructionDetailsT(this->_last_raw_instruction);
	}
};