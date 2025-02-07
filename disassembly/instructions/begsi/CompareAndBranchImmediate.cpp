#include "CompareAndBranchImmediate.h"
#include "../init_helpers.h"

InstructionDefs::Begsi::CompareAndBranchImmediate::CompareAndBranchImmediate(uint32_t encoded) :
	is64bit(i::val(encoded, 1, 31)),
	branchIfNonZero(i::val(encoded, 1, 24)),
	immediate(static_cast<int32_t>(i::val(encoded, 19, 5) << 2)),
	index(i::val(encoded, i::regindex_length_bits, 0)) {}
