#include "LoadStoreRegUnsignedImm.h"

LoadStoreRegUnsignedImm::LoadStoreRegUnsignedImm(uint32_t encoded) :
	size(8 << (encoded >> 30)),
	is_simd((encoded >> 26) & 1),
	is_using_64bit_reg((encoded >> 23) & 0b11),
	base_reg((encoded >> 5) & 0b11111),
	src_dst_reg(encoded & 0b11111),
	unsigned_imm((encoded >> 10) & 0b111111111111) {}
