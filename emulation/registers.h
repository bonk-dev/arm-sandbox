#pragma once

#include "emu_types.h"

namespace Emulation {
	enum class Registers : regindex_t {
		/**
		 * Stack pointer (X31)
		 */
		Sp = 31
	};
}