#include "Hint.h"

namespace {
    InstructionDefs::Begsi::Hint::Type decode_type(const uint32_t enc) {
        const uint8_t crm = (enc >> 8) & 0b1111;
        const uint8_t op2 = (enc >> 5) & 0b111;
        const uint8_t crmOp2Concat = crm | op2;

        return static_cast<InstructionDefs::Begsi::Hint::Type>(crmOp2Concat);
    }
}

InstructionDefs::Begsi::Hint::Hint(const uint32_t encoded) : hintType(decode_type(encoded)) {}
