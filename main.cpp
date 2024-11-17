#include <iostream>
#include <vector>
#include <format>
#include "disassembly/A64Decoder.h"
#include "disassembly/prettify.h"
#include "emulation/AArch64Cpu.h"
#include "emulation/executors/AddSubImmediateExecutor.h"
#include "emulation/executors/LoadStoreRegPairExecutor.h"
#include "emulation/executors/FormPcRelAddressExecutor.h"

template<class InstDetails>
void print_disassembly(InstDetails& i) {
	std::cout << "Disassembly: " << disassembly::to_pretty_string(i) << std::endl;
}

int main() {
	std::vector<std::byte> sample_code = {
			// ADD X25, X0, #0x7C0
			std::byte(0x19), std::byte(0x00), std::byte(0x1F), std::byte(0x91),

			// STP X29, X30, [SP, #-0x10]!
			std::byte(0xFD), std::byte(0x7B), std::byte(0xBF), std::byte(0xA9),

			// ADRP X0, #0
			std::byte(0x00), std::byte(0x00), std::byte(0x00), std::byte(0x90),

			// ADR X0, #0x2001
			std::byte(0x00), std::byte(0x00), std::byte(0x01), std::byte(0x30),

			// MOV X29, SP (alias of ADD X29, SP, #0)
			std::byte(0xFD), std::byte(0x03), std::byte(0x00), std::byte(0x91),

			// BL (offset -0x20)
			std::byte(0xF8), std::byte(0xFF), std::byte(0xFF), std::byte(0x97),

			// B (offset -0x20)
			std::byte(0xF8), std::byte(0xFF), std::byte(0xFF), std::byte(0x17)
	};
	A64Decoder dec(sample_code);

	constexpr size_t INITIAL_CPU_MEMORY = 10240; // bytes
	const auto shared_cpu = std::make_shared<AArch64Cpu>(INITIAL_CPU_MEMORY);
	AddSubImmediateExecutor add_sub_immediate_executor(shared_cpu);
	FormPcRelAddressExecutor form_pc_rel_address_executor(shared_cpu);
	LoadStoreRegPairExecutor load_store_pair_executor(shared_cpu);

	InstructionType inst = dec.decode_next();
	while (inst != InstructionType::Undefined) {
		switch (inst) {
			case InstructionType::AddOrSubImmediate:
			{
				AddImmediateInstruction details = dec.decode_add_immediate();
				print_disassembly(details);

				add_sub_immediate_executor.execute(details);
				break;
			}
			case InstructionType::PcRelativeAddressing:
			{
				FormPcRelAddressInstruction details = dec.decode_form_pc_rel_addr_instruction();
				print_disassembly(details);

				form_pc_rel_address_executor.execute(details);
				break;
			}
			case InstructionType::UnconditionalBranchImmediate:
			{
				UnconditionalBranchImmediateInstruction details = dec.decode_unconditional_branch_instruction();
				std::cout << "Decoded uncond branch" << std::endl;

				break;
			}
			case InstructionType::LoadStoreRegisterPair:
			{
				LoadStoreRegisterPairInstruction details = dec.decode_load_store_register_pair_instruction();
				print_disassembly(details);

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
