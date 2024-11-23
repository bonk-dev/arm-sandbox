#pragma once

#include "instructions/data_proc_imm/AddImmediate.h"
#include "instructions/data_proc_imm/FormPcRelAddress.h"
#include "instructions/UnconditionalBranchImmediate.h"
#include "instructions/loads_and_stores/LoadRegisterPair.h"
#include "instructions/MoveWideImmediate.h"

namespace disassembly {
	std::string to_pretty_string(InstructionDefs::DataProcImm::AddImmediate& i);
	std::string to_pretty_string(InstructionDefs::DataProcImm::FormPcRelAddress& i);
	std::string to_pretty_string(InstructionDefs::MoveWideImmediate& i);
	std::string to_pretty_string(InstructionDefs::LoadsAndStores::LoadRegisterPair& i);
	std::string to_pretty_string(InstructionDefs::UnconditionalBranchImmediate& i);
}