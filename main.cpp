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
#include "emulation/executors/MoveWideImmediateExecutor.h"
#include "emulation/executors/loads_and_stores/LoadStoreRegUnsignedImm.h"
#include "loaders/elf/ElfLoader.h"
#include "disassembly/instructions/begsi/UnconditionalBranchRegister.h"
#include "emulation/executors/UnconditionalBranchRegisterExecutor.h"
#include "emulation/executors/reserved/ReservedCallExecutor.h"

template<class InstDetails>
void print_disassembly(InstDetails& i) {
	std::cout << "Disassembly: " << disassembly::to_pretty_string(i) << std::endl;
}

template<typename ExecutorType>
void map_e(std::map<InstructionType, std::unique_ptr<ExecutorBase>>& map, InstructionType instructionType, const std::shared_ptr<AArch64Cpu>& c) {
	map[instructionType] = std::make_unique<ExecutorType>(c);
}

std::map<InstructionType, std::unique_ptr<ExecutorBase>> map_all_executors(const std::shared_ptr<AArch64Cpu>& sharedCpu) {
	std::map<InstructionType, std::unique_ptr<ExecutorBase>> executors;

	map_e<AddSubImmediateExecutor>(executors, InstructionType::AddOrSubImmediate, sharedCpu);
	map_e<FormPcRelAddressExecutor>(executors, InstructionType::PcRelativeAddressing, sharedCpu);
	map_e<MoveWideImmediateExecutor>(executors, InstructionType::MoveWideImmediate, sharedCpu);
	map_e<UnconditionalBranchImmediateExecutor>(executors, InstructionType::UnconditionalBranchImmediate, sharedCpu);
	map_e<UnconditionalBranchRegisterExecutor>(executors, InstructionType::UnconditionalBranchRegister, sharedCpu);
	map_e<LoadStoreRegPairExecutor>(executors, InstructionType::LoadStoreRegisterPair, sharedCpu);
	map_e<Executors::LoadsAndStores::LoadStoreRegUnsignedImm>(executors, InstructionType::LoadStoreRegisterUnsignedImm, sharedCpu);
	map_e<Executors::Reserved::ReservedCallExecutor>(executors, InstructionType::ReservedCall, sharedCpu);

	return executors;
}

int prototype_main() {
	std::vector<std::byte> sample_code = {
			// UDF #1 (special library call)
			std::byte(0x01), std::byte(0x00), std::byte(0x00), std::byte(0x00),

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

	const auto shared_cpu = std::make_shared<AArch64Cpu>();
	const auto executors = map_all_executors(shared_cpu);

	A64Decoder dec{};
	auto* dataPtr = reinterpret_cast<uint32_t*>(sample_code.data());
	InstructionType inst = dec.decodeNextType(*(dataPtr++));
	while (inst != InstructionType::Undefined) {
		auto& exec = executors.at(inst);
		exec->decodeAndExecute(dec.getRawInstruction());

		inst = dec.decodeNextType(*(dataPtr++));
	}

	return 0;
}

int read_elf_main(const char* path) {
	Loaders::ElfLoader loader(path);
	loader.loadEntireFile();
	loader.parse();

	std::shared_ptr<AArch64Cpu> cpu = std::make_shared<AArch64Cpu>();
	loader.allocateSections(cpu->getMemory());

	// TODO: dont execute until we do some linking
	return 2;

	// Normally we would start executing at "entry" - this offset is in the ELF header
	// but we are going to cheat for now
	constexpr virtual_address_t intMainOffset = 0x640;
	cpu->setProgramCounter(intMainOffset);

	auto executors = map_all_executors(cpu);
	A64Decoder dec{};
	InstructionType type;

	virtual_address_t pc = cpu->getProgramCounter();
	auto encodedInstruction = cpu->getMemory().read<uint32_t>(pc);
	type = dec.decodeNextType(encodedInstruction);

	while (type != InstructionType::Undefined) {
		std::cout << "[ElfMain] Program counter: " << std::hex << std::showbase << pc << std::endl;
		std::cout << "[ElfMain] Instruction: " << std::hex << std::showbase << encodedInstruction << std::endl;

		executors[type]->decodeAndExecute(encodedInstruction);

		virtual_address_t newPc = cpu->getProgramCounter();
		if (newPc == pc) {
			// if the program counter wasn't changed by an executor, increment it
			pc += sizeof(uint32_t);
			cpu->setProgramCounter(pc);
		}
		else {
			pc = newPc;
		}

		encodedInstruction = cpu->getMemory().read<uint32_t>(pc);
	 	type = dec.decodeNextType(encodedInstruction);
	}

	std::cerr << "[ElfMain] Undefined instruction: " << std::hex << std::showbase << encodedInstruction << std::endl;

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
