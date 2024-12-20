#include "ReservedCall.h"

InstructionDefs::Reserved::ReservedCall::ReservedCall(uint32_t encoded) :
	call_type(static_cast<InstructionDefs::Reserved::ReservedCalls>(encoded & 0b1111111111111111)) {}
