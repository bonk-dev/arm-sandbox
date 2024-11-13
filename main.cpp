#include <iostream>
#include <vector>
#include <cstdint>
#include "disassembly/A64Decoder.h"

int main() {
	std::vector<std::byte> sample_code = { std::byte(0x19), std::byte(0x00), std::byte(0x1F), std::byte(0x91) };
	A64Decoder dec(sample_code);

	InstructionType inst = dec.decode_next();
	while (inst != InstructionType::Undefined) {
		std::cout << "Instruction: " << static_cast<uint32_t>(inst) << std::endl;
		inst = dec.decode_next();
	}

	return 0;
}
