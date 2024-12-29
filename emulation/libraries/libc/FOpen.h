#pragma once

#include "../EmulatedSymbol.h"

namespace Emulation::Libraries::LibC {
	/**
	 * @brief Implements FILE *fopen( const char *restrict filename, const char *restrict mode );
	 * @see https://en.cppreference.com/w/c/io/fopen
	 */
	class FOpen final : public EmulatedSymbol {
	public:
		void execute(AArch64Cpu &cpu) override;
	};
}