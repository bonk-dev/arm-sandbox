#pragma once

#include "instructions/data_proc_imm/AddImmediate.h"
#include "instructions/data_proc_imm/FormPcRelAddress.h"
#include "instructions/begsi/UnconditionalBranchImmediate.h"
#include "instructions/loads_and_stores/LoadRegisterPair.h"
#include "instructions/data_proc_imm/MoveWideImmediate.h"

namespace disassembly {
	std::string to_pretty_string(InstructionDefs::DataProcImm::AddImmediate& i);
	std::string to_pretty_string(InstructionDefs::DataProcImm::FormPcRelAddress& i);
	std::string to_pretty_string(InstructionDefs::DataProcImm::MoveWideImmediate& i);
	std::string to_pretty_string(InstructionDefs::LoadsAndStores::LoadRegisterPair& i);
	std::string to_pretty_string(InstructionDefs::Begsi::UnconditionalBranchImmediate& i);
}