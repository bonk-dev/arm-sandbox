#pragma once

#include "A64Decoder.h"
#include "instructions/AddImmediateInstruction.h"
#include "instructions/FormPcRelAddressInstruction.h"

namespace disassembly {
	std::string to_pretty_string(AddImmediateInstruction& i);
	std::string to_pretty_string(FormPcRelAddressInstruction& i);
	std::string to_pretty_string(LoadRegisterPairInstruction& i);
	std::string to_pretty_string(UnconditionalBranchImmediateInstruction& i);
}