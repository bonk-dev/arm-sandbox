#pragma once
#include <cstdint>

namespace InstructionDefs {
	enum class LogicalOperation : uint8_t {
		// Bitwise AND
		And = 0b00,
		// Bitwise OR
		Or = 0b01,
		// Bitwise XOR
		Xor = 0b10,
		// Bitwise AND with condititon flag update
		AndSetFlags = 0b11
	};
}