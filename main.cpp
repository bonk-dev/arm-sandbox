#include <iostream>
#include <vector>
#include <cstdint>
#include "disassembly/A64Decoder.h"
#include "emulation/AArch64Cpu.h"

int main() {
	AArch64Cpu cpu{};
	cpu.write_gp_register_64(25, 0x1FFFFFFFF);
	std::cout << std::hex << cpu.read_gp_register_32(25) << std::endl;
	std::cout << std::hex << cpu.read_gp_register_64(25) << std::endl;
	cpu.write_gp_register_32(25, 0xBBBBBBBB);
	std::cout << std::hex << cpu.read_gp_register_32(25) << std::endl;
	std::cout << std::hex << cpu.read_gp_register_64(25) << std::endl;

	std::vector<std::byte> sample_code = { std::byte(0x19), std::byte(0x00), std::byte(0x1F), std::byte(0x91) };
	A64Decoder dec(sample_code);

	InstructionType inst = dec.decode_next();
	while (inst != InstructionType::Undefined) {
		switch (inst) {
			case InstructionType::AddOrSubImmediate:
			{
				AddImmediateInstruction details = dec.decode_add_immediate();
				printf("IMM: #0x%x, Destination index: %i, Src index: %i",
					   details.immediate, details.destination_reg_index, details.source_reg_index);
				break;
			}
			default:
				std::cout << "Invalid instruction" << std::endl;
				break;
		}

		inst = dec.decode_next();
	}

	return 0;
}
