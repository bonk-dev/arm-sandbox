#include <iostream>
#include <format>
#include <iomanip>
#include "disassembly/A64Decoder.h"
#include "disassembly/disassembly.h"
#include "emulation/AArch64Cpu.h"
#include "loaders/elf/ElfLoader.h"
#include "emulation/executors/all_executors.h"
#include "emulation/libraries/libc/LibCStartMain.h"
#include "emulation/libraries/libc/Puts.h"
#include "emulation/libraries/libc/FOpen.h"
#include "emulation/filesystem/EmulatedFile.h"
#include "emulation/libraries/libc/FScanF.h"

template<typename ExecutorType>
void map_e(std::map<InstructionType, std::unique_ptr<ExecutorBase>>& map, InstructionType instructionType, const std::shared_ptr<AArch64Cpu>& c) {
	map[instructionType] = std::make_unique<ExecutorType>(c);
}

template<typename ExecutorType, typename... Args>
void map_ep(
	std::map<InstructionType, std::unique_ptr<ExecutorBase>>& map,
	InstructionType instructionType,
	Args&&... args) {
	map[instructionType] = std::make_unique<ExecutorType>(std::forward<Args>(args)...);
}

std::map<InstructionType, std::unique_ptr<ExecutorBase>> map_all_executors(const std::shared_ptr<AArch64Cpu>& sharedCpu,
																		   const std::shared_ptr<Emulation::Libraries::Mapper>& mapper) {
	std::map<InstructionType, std::unique_ptr<ExecutorBase>> executors;

	map_e<Executors::DataProcImm::AddSubImmediateExecutor>(executors, InstructionType::AddOrSubImmediate, sharedCpu);
	map_e<Executors::DataProcImm::FormPcRelAddressExecutor>(executors, InstructionType::PcRelativeAddressing, sharedCpu);
	map_e<Executors::DataProcImm::MoveWideImmediateExecutor>(executors, InstructionType::MoveWideImmediate, sharedCpu);
	map_e<Executors::Begsi::ConditionalBranchImmediateExecutor>(executors, InstructionType::ConditionalBranchImmediate, sharedCpu);
	map_e<Executors::Begsi::UnconditionalBranchImmediateExecutor>(executors, InstructionType::UnconditionalBranchImmediate, sharedCpu);
	map_e<Executors::Begsi::UnconditionalBranchRegisterExecutor>(executors, InstructionType::UnconditionalBranchRegister, sharedCpu);
	map_e<Executors::LoadsAndStores::LoadStoreRegPairExecutor>(executors, InstructionType::LoadStoreRegisterPair, sharedCpu);
	map_e<Executors::LoadsAndStores::LoadStoreRegUnsignedImm>(executors, InstructionType::LoadStoreRegisterUnsignedImm, sharedCpu);
	map_ep<Executors::Reserved::ReservedCallExecutor>(executors, InstructionType::ReservedCall, sharedCpu, mapper);
	map_e<Executors::Begsi::HintExecutor>(executors, InstructionType::Hint, sharedCpu);
	map_e<Executors::DataProcReg::LogicalShiftedRegisterExecutor>(executors, InstructionType::LogicalShiftedRegister, sharedCpu);

	return executors;
}

int read_elf_main(const char* path) {
	Loaders::ElfLoader loader(path);
	loader.loadEntireFile();
	loader.parse();

	std::shared_ptr<AArch64Cpu> cpu = std::make_shared<AArch64Cpu>();
	loader.allocateSections(cpu->getMemory());

	const auto mapper = std::make_shared<Emulation::Libraries::Mapper>();

	// Register emulated functions
	mapper->registerLibraryImplementation(
		"__libc_start_main",
		std::make_unique<Emulation::Libraries::LibC::LibCStartMain>());
	mapper->registerLibraryImplementation(
		"puts",
		std::make_unique<Emulation::Libraries::LibC::Puts>());
	mapper->registerLibraryImplementation(
		"fopen",
		std::make_unique<Emulation::Libraries::LibC::FOpen>());
	mapper->registerLibraryImplementation(
			"__isoc23_fscanf",
			std::make_unique<Emulation::Libraries::LibC::FScanF>());

	// Dynamic link
	mapper->allocateLinkingSegment(cpu->getMemory());
	loader.linkSymbols(*mapper, cpu->getMemory());

	// Allocate clean exit instruction
	const virtual_address_t cleanExitAddr = cpu->getMemory().allocateSegment(sizeof(uint32_t));
	const InstructionDefs::Reserved::ReservedCall reservedCall(
		InstructionDefs::Reserved::ReservedCalls::Exit,
		InstructionDefs::Reserved::IMM_EXIT_CLEAN_EXIT);
	cpu->getMemory().write(cleanExitAddr, reservedCall.encode());
	cpu->setCleanExitAddress(cleanExitAddr);

	std::cout << "[ElfMain] Entry point: " << std::hex << std::showbase << loader.getEntryPoint() << std::endl;
	cpu->setProgramCounter(loader.getEntryPoint());

	auto executors = map_all_executors(cpu, mapper);
	A64Decoder dec{};
	InstructionType type;

	// TODO: this feels awkward
	// Add test file
	cpu->getFs().addFile("/tmp/test2.txt", std::make_shared<Filesystem::EmulatedFile>("asd", 0));

	std::cout << std::endl <<  "============ [main] Setup done. Starting the execution ============" << std::endl << std::endl;

	virtual_address_t pc = cpu->getProgramCounter();
	auto encodedInstruction = cpu->getMemory().read<uint32_t>(pc);
	type = dec.decodeNextType(encodedInstruction);

	while (type != InstructionType::Undefined) {
		std::cout << std::hex << std::noshowbase << "0x" << std::setfill('0') << std::setw(16) << pc << ": ";

		const auto& executor = executors.find(type);
		if (executor == executors.end()) {
			throw std::runtime_error(std::format("Instruction type \"{}\" does not have a valid executor!",
				static_cast<int>(type)));
		}
		executor->second->decodeAndExecute(dec.getRawInstruction());
		if (cpu->isHalted()) {
			break;
		}

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

	if (!cpu->isHalted()) {
		std::cerr << "[ElfMain] Undefined instruction: " << std::hex << std::showbase << encodedInstruction << std::endl;
	}
	else {
		std::cout << "[ElfMain] CPU halted. Exit code: " << cpu->getExitCode() << std::endl;
	}

	return 0;
}

int main(int argc, char** argv) {
	if (argc <= 1) {
		constexpr int InvalidUsage = 1;
		std::cout << "Usage: arm_sandbox <elf_file_path>" << std::endl;
		std::cout << "Example: arm_sandbox /home/bonk/hello_word" << std::endl;
		return InvalidUsage;
	}

	std::cout << "Loading ELF from " << argv[1] << std::endl;
	return read_elf_main(argv[1]);
}
