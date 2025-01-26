#include "debugger.h"
#include <memory>

namespace {
	std::unique_ptr<Logging::LoggerBase> logger = Logging::createLogger("debugger");
}

bool Cli::handle_manual_stepping_mode(ExecutorBase &executor, uint32_t instruction, AArch64Cpu &cpu) {
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
		} else if (cmd.starts_with("p")) {
			// print value of something
			if (cmd.length() < 4) {
				logger->error() << "Usage: p X0 or p X1 etc. (only reg printing is implemented)" << std::endl;
			} else {
				std::string arg = cmd.substr(2);
				int reg_size = 0;
				if (arg.starts_with('X') || arg.starts_with('x')) {
					reg_size = 64;
				} else if (arg.starts_with('W') || arg.starts_with('w')) {
					reg_size = 32;
				} else {
					logger->error() << "Invalid register" << std::endl;
				}

				if (reg_size > 0) {
					unsigned int index = std::stoul(arg.substr(1), nullptr, 10);
					if (index > AARCH64_GENERAL_PURPOSE_REGISTERS) {
						logger->error() << "Invalid register index" << std::endl;
					} else {
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
		} else {
			valid = false;
		}
	} while (!valid);

	return true;
}
