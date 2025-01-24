#include <format>
#include "InstructionUnallocatedError.h"

Emulation::Exceptions::InstructionUnallocatedError::InstructionUnallocatedError(const char *reason) :
	runtime_error(std::format("An unallocated instruction was decoded: {}", reason)) {}
