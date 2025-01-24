#include "../../../emulation/exceptions/InstructionUnallocatedError.h"
#include "AddSubExtendedRegister.h"
#include "../init_helpers.h"

InstructionDefs::DataProcReg::AddSubExtendedRegister::AddSubExtendedRegister(uint32_t encoded) :
	is64Bit(i::val(encoded, 1, 31)),
	subtract(i::val(encoded, 1, 30)),
	setFlags(i::val(encoded, 1, 29)),
	optField(i::val(encoded, 2, 22)),
	secondSourceReg(i::val(encoded, 5, 16)),
	extendVariant(i::val_enum<ExtendVariant>(encoded, 3, 13)),
	shiftAmount(i::val(encoded, 3, 10)),
	firstSourceReg(i::val(encoded, 5, 5)),
	destinationReg(i::val(encoded, 5, 0)) {
	if (optField != 00) {
		throw Emulation::Exceptions::InstructionUnallocatedError("Opt field cannot be non-zero");
	}
}
