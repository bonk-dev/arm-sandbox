#pragma once
#include "../emulation/executors/ExecutorBase.h"

namespace Cli {
	/**
	 * Handles the manual stepping mode commands and interaction
	 * @param executor The executor for the current instruction
	 * @param instruction The raw encoded instruction
	 * @param cpu The cpu instance
	 * @return Whether the hypervisor should stay in manual stepping mode
	 */
	bool handle_manual_stepping_mode(ExecutorBase &executor, uint32_t instruction, AArch64Cpu &cpu);
}