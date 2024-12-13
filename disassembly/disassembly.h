#pragma once

#include "instructions/data_proc_imm/AddImmediate.h"
#include "instructions/data_proc_imm/FormPcRelAddress.h"
#include "instructions/begsi/UnconditionalBranchImmediate.h"
#include "instructions/loads_and_stores/LoadRegisterPair.h"
#include "instructions/data_proc_imm/MoveWideImmediate.h"
#include "instructions/loads_and_stores/LoadStoreRegUnsignedImm.h"
#include "instructions/begsi/UnconditionalBranchRegister.h"

namespace disassembly {
	std::string to_pretty_string(const InstructionDefs::DataProcImm::AddImmediate& i);
	std::string to_pretty_string(const InstructionDefs::DataProcImm::FormPcRelAddress& i);
	std::string to_pretty_string(const InstructionDefs::DataProcImm::MoveWideImmediate& i);
	std::string to_pretty_string(const InstructionDefs::LoadsAndStores::LoadStoreRegUnsignedImm& i);
	std::string to_pretty_string(const InstructionDefs::LoadsAndStores::LoadRegisterPair& i);
	std::string to_pretty_string(const InstructionDefs::Begsi::UnconditionalBranchImmediate& i);
	std::string to_pretty_string(const InstructionDefs::Begsi::UnconditionalBranchRegister& i);
}