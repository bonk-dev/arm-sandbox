#pragma once

#include <cstdint>
#include "../../../emulation/emu_types.h"

typedef struct LoadStoreRegUnsignedImm {
	/**
	 * @var size
	 * @brief Size of loaded/stored data (in bits)
	 */
	uint8_t size;

	/**
	 * @var is_simd
	 * @brief If true, this is a SIMD operation (probably revolving around floating-point numbers)
	 */
	bool is_simd;

	/**
	 * @var is_using_64bit_reg
	 * @brief If true, read/writes are made to 64bit registers (Xn), 32bit registers (Wn) otherwise
	 */
	bool is_using_64bit_reg;

	union {
		int16_t signed_imm : 12;
		uint16_t unsigned_imm : 12;
	};

	/**
	 * @var src_dst_reg
	 * @brief Register index of which to store the value, or of which load a value into
	 */
	regindex_t src_dst_reg;

	/**
	 * @var base_reg
	 * @brief Register index containing the address for load/store
	 */
	regindex_t base_reg;

	explicit LoadStoreRegUnsignedImm(uint32_t encoded);

} LoadStoreRegUnsignedImm;