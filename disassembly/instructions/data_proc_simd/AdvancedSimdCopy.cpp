#include <stdexcept>
#include "AdvancedSimdCopy.h"
#include "../init_helpers.h"

namespace {
	using InstructionDefs::DataProcSimd::AdvancedSimdCopy;

	uint8_t decode_q(uint32_t e) {
		return i::val(e, 1, 30);
	}
	uint8_t decode_op(uint32_t e) {
		return i::val(e, 1, 29);
	}
	uint8_t decode_imm4(uint32_t e) {
		return i::val(e, 4, 11);
	}
	uint8_t decode_imm5(uint32_t e) {
		return i::val(e, 5, 16);
	}

	AdvancedSimdCopy::CopyType decode_copy_type(uint32_t e) {
		const uint32_t q = decode_q(e);
		const uint32_t op = decode_op(e);

		const auto i4 = decode_imm4(e);
		if (op == 0) {
			switch (i4) {
				case 0b0000:
					return AdvancedSimdCopy::CopyType::DuplicateElement;
				case 0b0001:
					return AdvancedSimdCopy::CopyType::DuplicateGeneral;
				default:
					break;
			}
			if (q == 0) {
				switch (i4) {
					case 0b0101:
						return AdvancedSimdCopy::CopyType::SignedMove;
					case 0b0111:
						return AdvancedSimdCopy::CopyType::UnsignedMove;
					default:
						break;
				}
			}
		}
		else if (q == 1) {
			if (op == 1) {
				return AdvancedSimdCopy::CopyType::InsertElement;
			}

			switch (i4) {
				case 0b0011:
					return AdvancedSimdCopy::CopyType::InsertGeneral;
				case 0b0101:
					return AdvancedSimdCopy::CopyType::SignedMove;
				case 0b111:
				{
					const auto i5 = decode_imm5(e);
					if ((i5 & 0b01111) == 0b01000) {
						return AdvancedSimdCopy::CopyType::UnsignedMove;
					}
					break;
				}
				default:
					break;
			}
		}

		throw std::runtime_error("Unallocated instruction");
	}
}

AdvancedSimdCopy::AdvancedSimdCopy(uint32_t encoded) :
	copyType(decode_copy_type(encoded)),
	_qField(i::val(encoded, 1, 30)),
	_imm5(i::val(encoded, 5, 16)),
	_imm4(i::val(encoded, 4, 11)),
	destinationRegIndex(i::val(encoded, i::regindex_length_bits, 0)),
	sourceRegIndex(i::val(encoded, i::regindex_length_bits, 5)) {

}

unsigned int InstructionDefs::DataProcSimd::AdvancedSimdCopy::getBaseSize() const {
	const uint8_t imm5Masked = _imm5 & 0b1111;
	return std::countr_zero(imm5Masked);
}

unsigned int InstructionDefs::DataProcSimd::AdvancedSimdCopy::getElementSize() const {
	const unsigned int baseSize = getBaseSize();
	return 8 << baseSize;
}

unsigned int InstructionDefs::DataProcSimd::AdvancedSimdCopy::getElementCount() const {
	switch (this->copyType) {
		case CopyType::DuplicateGeneral:
			return 1;
		case CopyType::DuplicateElement:
			return getDataSize() / getElementSize();
		default:
			throw std::runtime_error("Element count is only implemented for DUP instruction");
	}
}

unsigned int InstructionDefs::DataProcSimd::AdvancedSimdCopy::getDataSize() const {
	switch (this->copyType) {
		case CopyType::DuplicateGeneral:
			return getElementSize();
		case CopyType::DuplicateElement:
			return 64 << _qField;
		default:
			throw std::runtime_error("Data size is only implemented for DUP instruction");
	}
}

unsigned int InstructionDefs::DataProcSimd::AdvancedSimdCopy::getSourceElementIndex() const {
	// imm5<4:size+1>
	// size+1 -> mask
	constexpr uint8_t index_mask_lut[] = {
		0b10000, // RESERVED
		0b11000, // size+1 == 1
		0b11100, // size+1 == 2
		0b11110, // size+1 == 3
		0b11111  // size+1 == 4
	};
	const uint8_t mask = index_mask_lut[getBaseSize() + 1];
	return _imm5 & mask;
}

unsigned int InstructionDefs::DataProcSimd::AdvancedSimdCopy::getSourceElementIndexDataSize() const {
	return 64 << (_imm5 & 0b10000);
}
