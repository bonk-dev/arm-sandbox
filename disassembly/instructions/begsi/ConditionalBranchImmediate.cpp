#include "ConditionalBranchImmediate.h"

namespace {
	int32_t decode_immediate(uint32_t encoded) {
		return static_cast<int32_t>(((encoded >> 5) & 0b1111111111111111111)) * 4;
	}
	bool decode_is_consistent(uint32_t encoded) {
		return (encoded >> 4) & 1;
	}
	InstructionDefs::Begsi::ConditionalBranchImmediate::Condition decode_cond(uint32_t encoded) {
		return static_cast<InstructionDefs::Begsi::ConditionalBranchImmediate::Condition>(encoded & 0b1111);
	}
}

InstructionDefs::Begsi::ConditionalBranchImmediate::ConditionalBranchImmediate(uint32_t encoded) :
	immediate(decode_immediate(encoded)),
	isConsistent(decode_is_consistent(encoded)),
	condition(decode_cond(encoded)) {}
