#include <stdexcept>
#include "LoadStoreRegUnsignedImm.h"

namespace {
	InstructionDefs::IndexingMode decode_indexing_mode(uint32_t encoded) {
		const bool is_unsigned_offset = ((encoded >> 24) & 0b11) == 0b01;
		if (is_unsigned_offset) {
			return InstructionDefs::IndexingMode::UnsignedOffset;
		}

 		const uint8_t pre_post_bits = (encoded >> 10) & 0b11;
		switch (pre_post_bits) {
			case 0b01:
				return InstructionDefs::IndexingMode::PostIndex;
			case 0b11:
				return InstructionDefs::IndexingMode::PreIndex;
			default:
				throw std::runtime_error("Invalid indexing mode");
		}
	}
}

InstructionDefs::LoadsAndStores::LoadStoreRegUnsignedImm::LoadStoreRegUnsignedImm(uint32_t encoded) :
	indexing_mode(decode_indexing_mode(encoded)),
	size(8 << (encoded >> 30)),
	is_simd((encoded >> 26) & 1),
	is_using_64bit_reg((size == 64)),
	base_reg((encoded >> 5) & 0b11111),
	src_dst_reg(encoded & 0b11111),
	unsigned_imm((encoded >> 10) & 0b111111111111),
	unsigned_imm9((encoded >> 12 & 0b111111111)) {}
