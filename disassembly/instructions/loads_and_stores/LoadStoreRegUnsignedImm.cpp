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

	uint8_t decode_opc(uint32_t encoded) {
		return (encoded >> 22) & 0b11;
	}

	bool decode_is_simd(uint32_t encoded) {
		return (encoded >> 26) & 1;
	}

	bool decode_is_signed(uint32_t encoded) {
		const uint8_t opc_field = decode_opc(encoded);
		return opc_field >> 1;
	}

	uint8_t decode_size(uint32_t encoded) {
		return 8 << (encoded >> 30);
	}

	bool decode_is_load(uint32_t encoded) {
		const uint8_t opc_field = decode_opc(encoded);
		const bool is_simd = decode_is_simd(encoded);
		const bool is_signed = decode_is_signed(encoded);

		if (is_simd) {
			return opc_field & 1;
		}
		else {
			if (is_signed && decode_size(encoded) != 64) {
				return true;
			}

			return opc_field & 1;
		}
	}

	bool decode_is_using_64bit_reg(uint32_t encoded) {
		// TODO: SIMD
		// TODO: test this shit

		const uint8_t size = decode_size(encoded);
		if (size == 64) {
			return true;
		}

		const bool is_signed = decode_is_signed(encoded);
		if (is_signed) {
			if (size == 32) {
				return true;
			}

			const auto opc = decode_opc(encoded);
			return !(opc & 1);
		}

		return false;
	}
}

InstructionDefs::LoadsAndStores::LoadStoreRegUnsignedImm::LoadStoreRegUnsignedImm(uint32_t encoded) :
	indexing_mode(decode_indexing_mode(encoded)),
	scale(encoded >> 30),
	size(decode_size(encoded)),
	is_load(decode_is_load(encoded)),
	is_signed(decode_is_signed(encoded)),
	is_simd(decode_is_simd(encoded)),
	is_using_64bit_reg(decode_is_using_64bit_reg(encoded)),
	base_reg((encoded >> 5) & 0b11111),
	src_dst_reg(encoded & 0b11111),
	unsigned_imm(((encoded >> 10) & 0b111111111111) << scale),
	unsigned_imm9((encoded >> 12 & 0b111111111))
{
	const auto size_field = encoded >> 30;
	const auto vr_field = (encoded >> 26) & 1;
	const auto opc_field = (encoded >> 22) & 0b11;
	const auto opc_field_msb = opc_field >> 1;
	if ((encoded >> 30) != 0 && vr_field == 1 && opc_field_msb == 1) {
		throw std::runtime_error("Unallocated (size != 00, VR == 1, opc == 1x)");
	}

	if (size_field >> 1 == 1 && vr_field == 0 && opc_field == 0b11) {
		throw std::runtime_error("Unallocated (size == 1x, VR == 0, opc == 11)");
	}
}

bool InstructionDefs::LoadsAndStores::LoadStoreRegUnsignedImm::get_is_prefetch() const {
	return this->size == 64 && !this->is_simd && !this->is_load && this->is_signed;
}
