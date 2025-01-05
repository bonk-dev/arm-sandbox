#pragma once

#include <cstdint>
#include "../../../emulation/emu_types.h"
#include "../IndexingMode.h"

namespace InstructionDefs::LoadsAndStores {
	typedef struct LoadStoreRegUnsignedImm {
		/**
		 * @var indexing_mode
		 * @brief Defines how the address is calculated and wrote back (to the base register)
		 */
		InstructionDefs::IndexingMode indexing_mode;

		/**
		 * @var is_load
		 * @brief If true, the data is loaded from memory into a register
		 */
		bool is_load;

		/**
		 * @var is_signed
		 * @brief If true, the loaded data should be sign-extended
		 */
		bool is_signed;

		/**
		 * @var scale
		 * @brief Used to calculate data size, but also used in calculating offset
		 */
		uint8_t scale;

		/**
		 * @var size
		 * @brief Size of loaded/stored data (in bits)
		 */
		uint8_t size;

		/**
		 * @var is_simd
		 * @brief If true, this is a SIMD operation (revolving around floating-point numbers)
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

		union {
			int16_t signed_imm9 : 9;
			uint16_t unsigned_imm9 : 9;
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
		[[nodiscard]] bool get_is_prefetch() const;

	} LoadStoreRegUnsignedImm;
}
