#include "reserved.h"

InstructionType Decoding::decode_reserved(uint32_t raw_instruction) {
	const uint32_t op0 = raw_instruction >> 29 & 0b11;
	const uint32_t op1 = raw_instruction >> 16 & 0b111111111;

	// Normally this would be UDF ("permanently undefined"), but we are using it for emulating libraries
	// so kind of weird system call
	// UDF is when both op0 and op1 are both 0, but we are going to use the unallocated entry as well (op1 != 0)
	return op0 == 0
		   ? InstructionType::ReservedCall
		   : InstructionType::Undefined;
}
