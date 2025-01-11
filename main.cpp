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
#include "logging/LoggerContainer.h"
#include "cli/Options.h"
#include "cli/parser.h"
#include "cli/InteractiveMenu.h"

namespace {
	std::unique_ptr<Logging::LoggerBase> logger = Logging::createLogger("main");
}

std::map<InstructionType, std::unique_ptr<ExecutorBase>> map_all_executors(const std::shared_ptr<Emulation::Libraries::Mapper>& mapper) {
	std::map<InstructionType, std::unique_ptr<ExecutorBase>> e;

	e[InstructionType::AddOrSubImmediate] =
			std::make_unique<Executors::DataProcImm::AddSubImmediateExecutor>();
	e[InstructionType::PcRelativeAddressing] =
			std::make_unique<Executors::DataProcImm::FormPcRelAddressExecutor>();
	e[InstructionType::MoveWideImmediate] =
			std::make_unique<Executors::DataProcImm::MoveWideImmediateExecutor>();
	e[InstructionType::ConditionalBranchImmediate] =
			std::make_unique<Executors::Begsi::ConditionalBranchImmediateExecutor>();
	e[InstructionType::UnconditionalBranchImmediate] =
			std::make_unique<Executors::Begsi::UnconditionalBranchImmediateExecutor>();
	e[InstructionType::UnconditionalBranchRegister] =
			std::make_unique<Executors::Begsi::UnconditionalBranchRegisterExecutor>();
	e[InstructionType::Hint] =
			std::make_unique<Executors::Begsi::HintExecutor>();
	e[InstructionType::LoadStoreRegisterPair] =
			std::make_unique<Executors::LoadsAndStores::LoadStoreRegPairExecutor>();
	e[InstructionType::LoadStoreRegisterUnsignedImm] =
			std::make_unique<Executors::LoadsAndStores::LoadStoreRegUnsignedImm>();
	e[InstructionType::ReservedCall] =
			std::make_unique<Executors::Reserved::ReservedCallExecutor>(mapper);
	e[InstructionType::LogicalShiftedRegister] =
			std::make_unique<Executors::DataProcReg::LogicalShiftedRegisterExecutor>();

	return e;
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

int read_elf_main(const std::string& emulationTarget) {
	Loaders::ElfLoader loader(emulationTarget);
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
	std::string parseError;
	Cli::Options opt = Cli::parseOptions(argc, argv, parseError);
	if (!parseError.empty()) {
		logger->error() << "An error has occurred while parsing cli options: " << std::endl << parseError << std::endl;
		return 1;
	}
	if (opt.showHelp) {
		Cli::printUsage(*logger);
		return 0;
	}
	if (opt.interactive) {
		Cli::InteractiveMenu menu(opt);
		while (menu.menuLoop()) {

		}

		opt = menu.getOptions();
	}

	logger->verbose() << "Loading ELF from " << opt.emulationTarget << std::endl;
	return read_elf_main(opt.emulationTarget);
}
