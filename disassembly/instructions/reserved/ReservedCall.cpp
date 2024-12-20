#include "ReservedCall.h"

InstructionDefs::Reserved::ReservedCall::ReservedCall(uint32_t encoded) :
	call_type(static_cast<InstructionDefs::Reserved::ReservedCalls>(encoded & 0b1111111111111111)) {}

uint32_t InstructionDefs::Reserved::ReservedCall::encode() const {
	return static_cast<uint16_t>(this->call_type);
}
