#pragma once

#include "emu_types.h"

namespace Emulation {
	enum class Registers : regindex_t {
		/**
		 * Link register (X30)
		 */
		Lr = 30,
		/**
		 * Stack pointer (X31)
		 */
		Sp = 31
	};
}