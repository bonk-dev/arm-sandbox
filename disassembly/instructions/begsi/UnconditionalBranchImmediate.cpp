#include "UnconditionalBranchImmediate.h"

InstructionDefs::Begsi::UnconditionalBranchImmediate::UnconditionalBranchImmediate(uint32_t encoded) :
	is_with_link(encoded >> 31 & 1),
	immediate(static_cast<int32_t>(encoded & 0b11111111111111111111111111) << 2) {}
