#pragma once
#include "reserved_calls.h"

namespace InstructionDefs::Reserved {
	typedef struct ReservedCall {
		ReservedCalls call_type;

		explicit ReservedCall(uint32_t encoded);
		[[nodiscard]] uint32_t encode() const;
	} ReservedCall;
}