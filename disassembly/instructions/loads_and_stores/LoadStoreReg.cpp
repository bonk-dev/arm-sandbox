#include <stdexcept>
#include "LoadStoreReg.h"
#include "../init_helpers.h"
#include "../../../emulation/exceptions/FeatureNotImplemented.h"

namespace {
	InstructionDefs::IndexingMode decode_indexing_mode(const uint32_t enc) {
		const auto indexingVal = i::val(enc, 2, 10);
		switch (indexingVal) {
			case 0b00:
				return InstructionDefs::IndexingMode::SignedOffset;
			case 0b01:
				return InstructionDefs::IndexingMode::PostIndex;
			case 0b10:
				return InstructionDefs::IndexingMode::SignedOffset;
			case 0b11:
				return InstructionDefs::IndexingMode::PreIndex;
			default:
				throw std::runtime_error("Invalid indexing mode value");
		}
	}

	bool decode_is_unscaled(const uint32_t enc) {
		const auto indexingVal = i::val(enc, 2, 10);
		return indexingVal == 0b10 || indexingVal == 0b00;
	}
}

InstructionDefs::LoadsAndStores::LoadStoreReg::LoadStoreReg(uint32_t encoded) :
		size(8 << i::val(encoded, 2, 30)),
		isSimd(i::val(encoded, 1, 26)),
		encoding(decode_indexing_mode(encoded)),
		isUnscaledImm(decode_is_unscaled(encoded)),
		isLoad(i::val(encoded, 2, 22) != 0b00), // TODO: FEATURE_FP has different encoding for isLoad
		targetReg(i::val(encoded, 5, 0)),
		baseReg(i::val(encoded, 5, 5)),
		immediate(static_cast<int16_t>(i::val(encoded, 9, 12))) {
	if (isSimd) {
		throw Exceptions::FeatureNotImplemented("FP", "");
	}
}
