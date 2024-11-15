#include <iostream>
#include <vector>
#include <cstdint>
#include <format>
#include "disassembly/A64Decoder.h"
#include "emulation/AArch64Cpu.h"
#include "emulation/executors/AddSubImmediateExecutor.h"
#include "emulation/executors/LoadStoreRegPairExecutor.h"

int main() {
	std::vector<std::byte> sample_code = {
			// ADD X25, X0, #0x7C0
			std::byte(0x19), std::byte(0x00), std::byte(0x1F), std::byte(0x91),

			// STP X29, X30, [SP, #-0x10]!
			std::byte(0xFD), std::byte(0x7B), std::byte(0xBF), std::byte(0xA9),
	};
	A64Decoder dec(sample_code);

	const auto shared_cpu = std::make_shared<AArch64Cpu>(AArch64Cpu{});
	AddSubImmediateExecutor add_sub_immediate_executor(shared_cpu);
	LoadStoreRegPairExecutor load_store_pair_executor(shared_cpu);

	InstructionType inst = dec.decode_next();
	while (inst != InstructionType::Undefined) {
		switch (inst) {
			case InstructionType::AddOrSubImmediate:
			{
				AddImmediateInstruction details = dec.decode_add_immediate();
				printf("IMM: #0x%x, Destination index: %i, Src index: %i\n",
					   details.immediate, details.destination_reg_index, details.source_reg_index);

				add_sub_immediate_executor.execute(details);
				break;
			}
			case InstructionType::LoadStoreRegisterPair:
			{
				LoadStoreRegisterPairInstruction details = dec.decode_load_store_register_pair_instruction();

				std::cout << std::format("IMM: #[{:#x}], Base reg: {}, First: {}, Second: {}",
										 details.immediate_value, details.base_reg, details.first_reg_index, details.second_reg_index)
										 << std::endl;

				load_store_pair_executor.execute(details);
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
