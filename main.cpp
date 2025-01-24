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
	e[InstructionType::LogicalImmediate] =
			std::make_unique<Executors::DataProcImm::LogicalImmediateExecutor>();
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
	e[InstructionType::LoadStoreRegister] =
			std::make_unique<Executors::LoadsAndStores::LoadStoreRegExecutor>();
	e[InstructionType::LoadStoreRegisterUnsignedImm] =
			std::make_unique<Executors::LoadsAndStores::LoadStoreRegUnsignedImm>();
	e[InstructionType::ReservedCall] =
			std::make_unique<Executors::Reserved::ReservedCallExecutor>(mapper);
	e[InstructionType::AddSubExtendedRegister] =
			std::make_unique<Executors::DataProcReg::AddSubExtendedRegisterExecutor>();
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

/**
 * Handles the manual stepping mode commands and interaction
 * @param executor The executor for the current instruction
 * @param instruction The raw encoded instruction
 * @param cpu The cpu instance
 * @return Whether the hypervisor should stay in manual stepping mode
 */
bool handle_manual_stepping_mode(ExecutorBase& executor, uint32_t instruction, AArch64Cpu& cpu) {
	executor.decodeAndLog(instruction);
	logger->error(false) << std::endl;

	std::string cmd;
	bool valid;
	do {
		std::getline(std::cin, cmd);
		valid = true;

		if (cmd == "stepi") {
			// execute rest, stay in manualStepping mode
		} else if (cmd == "continue") {
			return false;
			// execute rest, exit manualStepping mode
		}
		else if (cmd.starts_with("p")) {
			// print value of something
			if (cmd.length() < 4) {
				logger->error() << "Usage: p X0 or p X1 etc. (only reg printing is implemented)" << std::endl;
			}
			else {
				std::string arg = cmd.substr(2);
				int reg_size = 0;
				if (arg.starts_with('X') || arg.starts_with('x')) {
					reg_size = 64;
				}
				else if (arg.starts_with('W') || arg.starts_with('w')) {
					reg_size = 32;
				}
				else {
					logger->error() << "Invalid register" << std::endl;
				}

				if (reg_size > 0) {
					unsigned int index = std::stoul(arg.substr(1), nullptr, 10);
					if (index > AARCH64_GENERAL_PURPOSE_REGISTERS) {
						logger->error() << "Invalid register index" << std::endl;
					}
					else {
						uint64_t reg_value = reg_size == 64
											 ? cpu.readRegister64(index, true)
											 : cpu.readRegister32(index, true);

						logger->error() << std::dec << std::noshowbase << 'X' << index << " = " << std::hex
										<< std::showbase << reg_value << std::endl;
					}
				}
			}

			// don't continue the execution if p command was used
			valid = false;
		}
		else {
			valid = false;
		}
	} while (!valid);

	return true;
}

int read_elf_main(const Cli::Options& options) {
	Loaders::ElfLoader loader(options.emulationTarget);
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

	bool manualStepping = false;
	while (type != InstructionType::Undefined) {
		logger->info() << std::hex << std::noshowbase << (manualStepping ? "next: " : "") << "0x" << std::setfill('0') << std::setw(16) << pc << ": ";

		const auto& executor = executors.find(type);
		if (executor == executors.end()) {
			throw std::runtime_error(std::format("Instruction type \"{}\" does not have a valid executor!",
												 static_cast<int>(type)));
		}
		if (!manualStepping && std::find(
				options.breakpoints.begin(), options.breakpoints.end(), pc) != options.breakpoints.end()) {
			manualStepping = true;

			logger->error(false) << std::endl << "============= Breakpoint hit =============" << std::endl;
			logger->error(false) << "Next instruction: ";
		}

		if (manualStepping) {
			manualStepping = handle_manual_stepping_mode(*executor->second, dec.getRawInstruction(), cpu);
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
		while (menu.menuLoop()) {}

		if (menu.getState() == Cli::MenuState::Exit) {
			logger->error() << "Exiting" << std::endl;
			return 0;
		}

		opt = menu.getOptions();
	}

	logger->verbose() << "Loading ELF from " << opt.emulationTarget << std::endl;
	return read_elf_main(opt);
}
