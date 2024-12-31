#pragma once

#include "../EmulatedSymbol.h"

namespace Emulation::Libraries::LibC {
	/**
	 * @brief Implements int fscanf( FILE *restrict stream, const char *restrict format, ... );
	 * Only supports one destination parameter :(
	 * @see https://en.cppreference.com/w/c/io/fscanf
	 */
	class FScanF final : public EmulatedSymbol {
	public:
		void execute(AArch64Cpu &cpu) override;
	};
}
