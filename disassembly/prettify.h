#pragma once

#include "instructions/AddImmediateInstruction.h"
#include "instructions/FormPcRelAddressInstruction.h"
#include "instructions/UnconditionalBranchImmediateInstruction.h"
#include "instructions/LoadRegisterPairInstruction.h"
#include "instructions/MoveWideImmediateInstruction.h"

namespace disassembly {
	std::string to_pretty_string(InstructionDefs::AddImmediateInstruction& i);
	std::string to_pretty_string(InstructionDefs::FormPcRelAddressInstruction& i);
	std::string to_pretty_string(InstructionDefs::MoveWideImmediateInstruction& i);
	std::string to_pretty_string(InstructionDefs::LoadRegisterPairInstruction& i);
	std::string to_pretty_string(InstructionDefs::UnconditionalBranchImmediateInstruction& i);
}