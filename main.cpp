#include <iostream>
#include <vector>
#include <format>
#include "disassembly/A64Decoder.h"
#include "disassembly/prettify.h"
#include "emulation/AArch64Cpu.h"
#include "emulation/executors/AddSubImmediateExecutor.h"
#include "emulation/executors/LoadStoreRegPairExecutor.h"
#include "emulation/executors/FormPcRelAddressExecutor.h"
#include "emulation/executors/UnconditionalBranchImmediateExecutor.h"
#include "disassembly/instructions/AddImmediate.h"
#include "disassembly/instructions/FormPcRelAddress.h"
#include "disassembly/instructions/MoveWideImmediate.h"
#include "emulation/executors/MoveWideImmediateExecutor.h"

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
			std::byte(0xF8), std::byte(0xFF), std::byte(0xFF), std::byte(0x17),

			// MOV W0, #0 (alias of MOVZ W0, #0)
			std::byte(0x00), std::byte(0x00), std::byte(0x80), std::byte(0x52),

			// MOV W15, #55 (alias of MOVZ W15, #55)
			std::byte(0xEF), std::byte(0x06), std::byte(0x80), std::byte(0x52),

			// LDP X29, X30, [SP], #0x10
			std::byte(0xFD), std::byte(0x7B), std::byte(0xC1), std::byte(0xA8)
	};
	A64Decoder dec(sample_code);

	constexpr size_t INITIAL_CPU_MEMORY = 10240; // bytes
	const auto shared_cpu = std::make_shared<AArch64Cpu>(INITIAL_CPU_MEMORY);
	AddSubImmediateExecutor add_sub_immediate_executor(shared_cpu);
	FormPcRelAddressExecutor form_pc_rel_address_executor(shared_cpu);
	MoveWideImmediateExecutor move_wide_imm_executor(shared_cpu);
	UnconditionalBranchImmediateExecutor unconditional_branch_imm_executor(shared_cpu);
	LoadStoreRegPairExecutor load_store_pair_executor(shared_cpu);

	InstructionType inst = dec.decode_next();
	while (inst != InstructionType::Undefined) {
		switch (inst) {
			case InstructionType::AddOrSubImmediate:
			{
				auto details = dec.decode_details<InstructionDefs::AddImmediate>();
				print_disassembly(details);

				add_sub_immediate_executor.execute(details);
				break;
			}
			case InstructionType::PcRelativeAddressing:
			{
				auto details = dec.decode_details<InstructionDefs::FormPcRelAddress>();
				print_disassembly(details);

				form_pc_rel_address_executor.execute(details);
				break;
			}
			case InstructionType::MoveWideImmediate:
			{
				auto details = dec.decode_details<InstructionDefs::MoveWideImmediate>();
				print_disassembly(details);

				move_wide_imm_executor.execute(details);
				break;
			}
			case InstructionType::UnconditionalBranchImmediate:
			{
				auto details = dec.decode_details<InstructionDefs::UnconditionalBranchImmediate>();
				print_disassembly(details);

				unconditional_branch_imm_executor.execute(details);
				break;
			}
			case InstructionType::LoadStoreRegisterPair:
			{
				auto details = dec.decode_details<InstructionDefs::LoadStoreRegisterPairInstruction>();
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
