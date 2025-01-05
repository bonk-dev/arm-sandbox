#pragma once

#include "instructions/begsi/Hint.h"
#include "instructions/data_proc_imm/AddImmediate.h"
#include "instructions/data_proc_imm/FormPcRelAddress.h"
#include "instructions/begsi/ConditionalBranchImmediate.h"
#include "instructions/begsi/UnconditionalBranchImmediate.h"
#include "instructions/loads_and_stores/LoadStoreRegisterPair.h"
#include "instructions/data_proc_imm/MoveWideImmediate.h"
#include "instructions/loads_and_stores/LoadStoreRegUnsignedImm.h"
#include "instructions/begsi/UnconditionalBranchRegister.h"
#include "instructions/data_proc_register/LogicalShiftedRegister.h"
#include "instructions/reserved/ReservedCall.h"

namespace disassembly {
	std::string to_pretty_string(const InstructionDefs::DataProcImm::AddImmediate& i);
	std::string to_pretty_string(const InstructionDefs::DataProcImm::FormPcRelAddress& i);
	std::string to_pretty_string(const InstructionDefs::DataProcImm::MoveWideImmediate& i);
	std::string to_pretty_string(const InstructionDefs::LoadsAndStores::LoadStoreRegUnsignedImm& i);
	std::string to_pretty_string(const InstructionDefs::LoadsAndStores::LoadStoreRegisterPair& i);
	std::string to_pretty_string(const InstructionDefs::Begsi::Hint& i);
	std::string to_pretty_string(const InstructionDefs::Begsi::ConditionalBranchImmediate& i);
	std::string to_pretty_string(const InstructionDefs::Begsi::UnconditionalBranchImmediate& i);
	std::string to_pretty_string(const InstructionDefs::Begsi::UnconditionalBranchRegister& i);
	std::string to_pretty_string(const InstructionDefs::DataProcReg::LogicalShiftedRegister& i);
	std::string to_pretty_string(const InstructionDefs::Reserved::ReservedCall& i);
}