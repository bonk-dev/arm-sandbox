#pragma once
#include <stdexcept>

namespace Emulation::Exceptions {
	/**
	 * @brief Thrown when an unallocated instruction was decoded
	 */
	class InstructionUnallocatedError final : public std::runtime_error {
	public:
		explicit InstructionUnallocatedError(const char* reason);
	};
}

