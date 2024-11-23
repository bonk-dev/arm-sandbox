#pragma once

#include "instructions/AddImmediate.h"
#include "instructions/FormPcRelAddress.h"
#include "instructions/UnconditionalBranchImmediate.h"
#include "instructions/LoadRegisterPair.h"
#include "instructions/MoveWideImmediate.h"

namespace disassembly {
	std::string to_pretty_string(InstructionDefs::AddImmediate& i);
	std::string to_pretty_string(InstructionDefs::FormPcRelAddress& i);
	std::string to_pretty_string(InstructionDefs::MoveWideImmediate& i);
	std::string to_pretty_string(InstructionDefs::LoadRegisterPair& i);
	std::string to_pretty_string(InstructionDefs::UnconditionalBranchImmediate& i);
}