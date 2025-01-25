#pragma once
#include <ranges>
#include <vector>
#include <map>
#include "CpuStack.h"

namespace Emulation {
	struct SystemVStackInitInfo {
		std::vector<std::string> args;
		std::map<std::string, std::string> environmentVariables;
	};

	/**
	 * @brief Pushes the environment variables, program arguments and their count onto the stack (accordingly to the System V ABI)
	 * @param stack The stack to push onto
	 * @param initInfo The struct containing the env vars and args
	 */
	void initialize_system_v_stack(CpuStack &stack, const SystemVStackInitInfo& initInfo) {
		// TODO: auxiliary

		// Environment variables (envp)
		stack.push(0x00UL); // 64bit NULL (terminate envp)
		for (const auto& envVar : std::ranges::reverse_view(initInfo.environmentVariables)) {
			const std::string concat = envVar.first + '=' + envVar.second;
			stack.push(concat);
		}

		// Program arguments (argv)
		stack.push(0x00UL); // 64bit NULL (terminate argv)
		for (const auto& arg : std::ranges::reverse_view(initInfo.args)) {
			stack.push(arg);
		}

		// Argument count (argc)
		stack.push(static_cast<uint64_t>(initInfo.args.size()));
	}
}