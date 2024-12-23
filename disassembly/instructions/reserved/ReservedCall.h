#pragma once
#include "reserved_calls.h"

namespace InstructionDefs::Reserved {
	/**
	 * @brief Non-standard instruction made up by me to make library emulation easier
	 *
	 * Encoding: 0 00 0000 [call_type (9 bits)] [immediate (16 bits)]
	 */
	typedef struct ReservedCall {
		ReservedCalls call_type;
		uint16_t immediate;

		explicit ReservedCall(uint32_t encoded);
		ReservedCall(InstructionDefs::Reserved::ReservedCalls callType, uint16_t imm);
		[[nodiscard]] uint32_t encode() const;
	} ReservedCall;
}