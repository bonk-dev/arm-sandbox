#pragma once

#include <cstdint>

namespace InstructionDefs {
	enum class IndexingMode : uint8_t {
		PostIndex,
		PreIndex,
		SignedOffset,
		UnsignedOffset,
		NonTemporalOffset,
	};
}
