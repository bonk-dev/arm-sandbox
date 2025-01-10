#include <format>
#include <iomanip>
#include "disassembly/A64Decoder.h"
#include "emulation/AArch64Cpu.h"
#include "loaders/elf/ElfLoader.h"
#include "emulation/executors/all_executors.h"
#include "emulation/libraries/libc/LibCStartMain.h"
#include "emulation/libraries/libc/Puts.h"
#include "emulation/libraries/libc/FOpen.h"
#include "emulation/filesystem/EmulatedFile.h"
#include "emulation/libraries/libc/FScanF.h"
#include "logging/LoggerBase.h"
#include "logging/LoggerFactory.h"

namespace {
	std::unique_ptr<Logging::LoggerBase> logger = Logging::createLogger("main");
}

template<typename ExecutorType>
void map_e(std::map<InstructionType, std::unique_ptr<ExecutorBase>>& map, InstructionType instructionType) {
	map[instructionType] = std::make_unique<ExecutorType>();
}

template<typename ExecutorType, typename... Args>
void map_ep(
	std::map<InstructionType, std::unique_ptr<ExecutorBase>>& map,
	InstructionType instructionType,
	Args&&... args) {
	map[instructionType] = std::make_unique<ExecutorType>(std::forward<Args>(args)...);
}

std::map<InstructionType, std::unique_ptr<ExecutorBase>> map_all_executors(const std::shared_ptr<Emulation::Libraries::Mapper>& mapper) {
	std::map<InstructionType, std::unique_ptr<ExecutorBase>> executors;

	map_e<Executors::DataProcImm::AddSubImmediateExecutor>(executors, InstructionType::AddOrSubImmediate);
	map_e<Executors::DataProcImm::FormPcRelAddressExecutor>(executors, InstructionType::PcRelativeAddressing);
	map_e<Executors::DataProcImm::MoveWideImmediateExecutor>(executors, InstructionType::MoveWideImmediate);
	map_e<Executors::Begsi::ConditionalBranchImmediateExecutor>(executors, InstructionType::ConditionalBranchImmediate);
	map_e<Executors::Begsi::UnconditionalBranchImmediateExecutor>(executors, InstructionType::UnconditionalBranchImmediate);
	map_e<Executors::Begsi::UnconditionalBranchRegisterExecutor>(executors, InstructionType::UnconditionalBranchRegister);
	map_e<Executors::LoadsAndStores::LoadStoreRegPairExecutor>(executors, InstructionType::LoadStoreRegisterPair);
	map_e<Executors::LoadsAndStores::LoadStoreRegUnsignedImm>(executors, InstructionType::LoadStoreRegisterUnsignedImm);
	map_ep<Executors::Reserved::ReservedCallExecutor>(executors, InstructionType::ReservedCall, mapper);
	map_e<Executors::Begsi::HintExecutor>(executors, InstructionType::Hint);
	map_e<Executors::DataProcReg::LogicalShiftedRegisterExecutor>(executors, InstructionType::LogicalShiftedRegister);

	return executors;
}

void register_library_implementations(Emulation::Libraries::Mapper& mapper) {
	// Register emulated functions
	mapper.registerLibraryImplementation(
			"__libc_start_main",
			std::make_unique<Emulation::Libraries::LibC::LibCStartMain>());
	mapper.registerLibraryImplementation(
			"puts",
			std::make_unique<Emulation::Libraries::LibC::Puts>());
	mapper.registerLibraryImplementation(
			"fopen",
			std::make_unique<Emulation::Libraries::LibC::FOpen>());
	mapper.registerLibraryImplementation(
			"__isoc23_fscanf",
			std::make_unique<Emulation::Libraries::LibC::FScanF>());
}

int read_elf_main(const char* path) {
	Loaders::ElfLoader loader(path);
	loader.loadEntireFile();
	loader.parse();

	AArch64Cpu cpu{};
	loader.allocateSections(cpu.getMemory());

	const auto mapper = std::make_shared<Emulation::Libraries::Mapper>();
	register_library_implementations(*mapper);

	// Dynamic link
	mapper->allocateLinkingSegment(cpu.getMemory());
	loader.linkSymbols(*mapper, cpu.getMemory());

	// Allocate clean exit instruction
	const virtual_address_t cleanExitAddr = cpu.getMemory().allocateSegment(sizeof(uint32_t));
	const InstructionDefs::Reserved::ReservedCall reservedCall(
		InstructionDefs::Reserved::ReservedCalls::Exit,
		InstructionDefs::Reserved::IMM_EXIT_CLEAN_EXIT);
	cpu.getMemory().write(cleanExitAddr, reservedCall.encode());
	cpu.setCleanExitAddress(cleanExitAddr);

	logger->info() << "Entry point: " << std::hex << std::showbase << loader.getEntryPoint() << std::endl;
	cpu.setProgramCounter(loader.getEntryPoint());

	auto executors = map_all_executors(mapper);
	A64Decoder dec{};

	// Add test file
	cpu.getFs().addFile("/tmp/test2.txt", std::make_shared<Filesystem::EmulatedFile>("asd"));

	logger->info() << std::endl <<  "============ Setup done. Starting the execution ============" << std::endl << std::endl;

	virtual_address_t pc = cpu.getProgramCounter();
	auto encodedInstruction = cpu.getMemory().read<uint32_t>(pc);
	InstructionType type = dec.decodeNextType(encodedInstruction);

	while (type != InstructionType::Undefined) {
		logger->info() << std::hex << std::noshowbase << "0x" << std::setfill('0') << std::setw(16) << pc << ": ";

		const auto& executor = executors.find(type);
		if (executor == executors.end()) {
			throw std::runtime_error(std::format("Instruction type \"{}\" does not have a valid executor!",
				static_cast<int>(type)));
		}
		executor->second->decodeAndExecute(dec.getRawInstruction(), cpu);
		if (cpu.isHalted()) {
			break;
		}

		virtual_address_t newPc = cpu.getProgramCounter();
		if (newPc == pc) {
			// if the program counter wasn't changed by an executor, increment it
			pc += sizeof(uint32_t);
			cpu.setProgramCounter(pc);
		}
		else {
			pc = newPc;
		}

		encodedInstruction = cpu.getMemory().read<uint32_t>(pc);
	 	type = dec.decodeNextType(encodedInstruction);
	}

	if (!cpu.isHalted()) {
		logger->error() << "Undefined instruction: " << std::hex << std::showbase << encodedInstruction << std::endl;
	}
	else {
		logger->info() << "CPU halted. Exit code: " << cpu.getExitCode() << std::endl;
	}

	return 0;
}

int main(int argc, char** argv) {
	if (argc <= 1) {
		constexpr int InvalidUsage = 1;
		logger->error() << "Usage: arm_sandbox <elf_file_path>" << std::endl;
		logger->error() << "Example: arm_sandbox /home/bonk/hello_word" << std::endl;
		return InvalidUsage;
	}

	logger->verbose() << "Loading ELF from " << argv[1] << std::endl;
	return read_elf_main(argv[1]);
}
