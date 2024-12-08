#include <iostream>
#include <vector>
#include <format>
#include <memory>
#include "disassembly/A64Decoder.h"
#include "disassembly/disassembly.h"
#include "emulation/AArch64Cpu.h"
#include "emulation/executors/AddSubImmediateExecutor.h"
#include "emulation/executors/LoadStoreRegPairExecutor.h"
#include "emulation/executors/FormPcRelAddressExecutor.h"
#include "emulation/executors/UnconditionalBranchImmediateExecutor.h"
#include "disassembly/instructions/data_proc_imm/AddImmediate.h"
#include "disassembly/instructions/data_proc_imm/FormPcRelAddress.h"
#include "disassembly/instructions/data_proc_imm/MoveWideImmediate.h"
#include "emulation/executors/MoveWideImmediateExecutor.h"
#include "emulation/executors/loads_and_stores/LoadStoreRegUnsignedImm.h"
#include "loaders/elf/ElfLoader.h"

template<class InstDetails>
void print_disassembly(InstDetails& i) {
	std::cout << "Disassembly: " << disassembly::to_pretty_string(i) << std::endl;
}

template<typename ExecutorType>
void map_e(std::map<InstructionType, std::unique_ptr<ExecutorBase>>& map, InstructionType instructionType, const std::shared_ptr<AArch64Cpu>& c) {
	map[instructionType] = std::make_unique<ExecutorType>(c);
}

int prototype_main() {
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
			std::byte(0xFD), std::byte(0x7B), std::byte(0xC1), std::byte(0xA8),

			// LDR X17, [X16, #0x20]
			std::byte(0x11), std::byte(0x12), std::byte(0x40), std::byte(0xF9)
	};

	constexpr size_t INITIAL_CPU_MEMORY = 10240; // bytes
	const auto shared_cpu = std::make_shared<AArch64Cpu>(INITIAL_CPU_MEMORY);

	std::map<InstructionType, std::unique_ptr<ExecutorBase>> executors;
	map_e<AddSubImmediateExecutor>(executors, InstructionType::AddOrSubImmediate, shared_cpu);
	map_e<FormPcRelAddressExecutor>(executors, InstructionType::PcRelativeAddressing, shared_cpu);
	map_e<MoveWideImmediateExecutor>(executors, InstructionType::MoveWideImmediate, shared_cpu);
	map_e<UnconditionalBranchImmediateExecutor>(executors, InstructionType::UnconditionalBranchImmediate, shared_cpu);
	map_e<LoadStoreRegPairExecutor>(executors, InstructionType::LoadStoreRegisterPair, shared_cpu);
	map_e<Executors::LoadsAndStores::LoadStoreRegUnsignedImm>(executors, InstructionType::LoadStoreRegisterUnsignedImm, shared_cpu);

	A64Decoder dec(sample_code);
	InstructionType inst = dec.decode_next();
	while (inst != InstructionType::Undefined) {
		auto& exec = executors.at(inst);
		exec->decodeAndExecute(dec.getRawInstruction());

		inst = dec.decode_next();
	}

	return 0;
}

int read_elf_main(const char* path) {
	Loaders::ElfLoader loader(path);
	loader.loadEntireFile();
	loader.parse();

	CpuVirtualMemory mem{};
	loader.allocateSections(mem);

	// Normally we would start executing at "entry" - this offset is in the ELF header
	// but we are going to cheat for now
	constexpr virtual_address_t intMainOffset = 0x640;

	for (virtual_address_t i = intMainOffset; i <= intMainOffset + 0x20; ++i) {
		std::cout << std::format("{:#04x} ", static_cast<int>(mem.read<uint8_t>(i)));
		if (i % 4 == 3) {
			std::cout << std::endl;
		}
	}

	return 0;
}

int main(int argc, char** argv) {
	if (argc <= 1) {
		std::cout << "No arguments provided. Entering prototype mode..." << std::endl;
		return prototype_main();
	}
	else {
		std::cout << "Loading ELF from " << argv[1] << std::endl;
		return read_elf_main(argv[1]);
	}
}
