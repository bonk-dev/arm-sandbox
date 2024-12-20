#include "ReservedCall.h"

namespace {
	constexpr unsigned int RESERVED_CALL_TYPE_INDEX = 16;
	constexpr unsigned int RESERVED_CALL_TYPE_LENGTH = 9;
	constexpr unsigned int RESERVED_CALL_TYPE_MASK = (2 << RESERVED_CALL_TYPE_LENGTH) - 1;
	constexpr unsigned int IMMEDIATE_MASK = (2 << 16) - 1;
}

InstructionDefs::Reserved::ReservedCall::ReservedCall(uint32_t encoded) :
	call_type(static_cast<InstructionDefs::Reserved::ReservedCalls>((encoded >> RESERVED_CALL_TYPE_INDEX) & RESERVED_CALL_TYPE_MASK)),
	immediate(encoded & IMMEDIATE_MASK) {}

uint32_t InstructionDefs::Reserved::ReservedCall::encode() const {
	uint32_t enc = (static_cast<uint16_t>(this->call_type)) << RESERVED_CALL_TYPE_INDEX;
	enc |= this->immediate;
	return enc;
}
