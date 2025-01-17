#include "LogicalShiftedRegister.h"
#include "../init_helpers.h"

InstructionDefs::DataProcReg::LogicalShiftedRegister::LogicalShiftedRegister(const uint32_t enc) :
    is64Bit(i::val(enc, 1, 31)),
    operation(i::val_enum<InstructionDefs::LogicalOperation>(enc, 2, 29)),
    shiftType(i::val_enum<ShiftType>(enc, 2, 22)),
    negate(i::val(enc, 1, 21)),
    operand2Reg(i::val(enc, 5, 16)),
    shiftAmount(i::val(enc, 6, 10)),
    operand1Reg(i::val(enc, 5, 5)),
    destinationReg(i::val(enc, 5, 0)) {}
