#include <sstream>
#include <format>
#include "disassembly.h"

void insert_signed_hex(std::stringstream& ss, int32_t val) {
	ss << std::hex << std::showbase;
	if (val >= 0) {
		ss << val;
	}
	else {
		val = abs(val);
		ss << "-" << val;
	}
	ss << std::noshowbase;
}

std::string gp_reg_name(regindex_t index, unsigned int bitness = 64) {
	if (bitness != 64 && bitness != 32) {
		throw std::runtime_error("reg_name only supports bitness of 64 and 32");
	}

	switch (index) {
		case 30:
			return bitness == 64 ? "LP" : "LPW";
		case 31:
			return bitness == 64 ? "SP" : "SPW";
		default:
			const char bit_prefix = bitness == 64
									? 'X'
									: 'W';
			return std::format("{}{}", bit_prefix, toascii(index));
	}
}
std::string gp_reg_name(regindex_t index, bool is_64bit) {
	return gp_reg_name(index, static_cast<unsigned int>(is_64bit ? 64 : 32));
}

/**
 * Returns the stringified name of the register, but interprets index 31 as the zero registers
 * @param index Register name
 * @param bitness Register bitness
 * @return Actual name of the register
 */
std::string gp_reg_name_zero(regindex_t index, unsigned int bitness = 64) {
	if (bitness != 64 && bitness != 32) {
		throw std::runtime_error("reg_name only supports bitness of 64 and 32");
	}

	if (index == 31) {
		return bitness == 64 ? "XZR" : "WZR";
	}

	return gp_reg_name(index, bitness);
}

/**
 * Returns the stringified name of the register, but interprets index 31 as the zero registers
 * @param index Register name
 * @param is_64bit True if operating on 64-bit registers, false if operating on 32-bit registers
 * @return Actual name of the register
 */
std::string gp_reg_name_zero(regindex_t index, bool is_64bit) {
	return gp_reg_name_zero(index, static_cast<unsigned int>(is_64bit ? 64 : 32));
}

std::string disassembly::to_pretty_string(const InstructionDefs::DataProcImm::AddImmediate &i) {
	std::stringstream ss;
	if (i.set_flags) {
		ss << (i.is_subtraction
			   ? "SUBS"
			   : "ADDS");
	}
	else if ((i.destination_reg_index == 31 || i.source_reg_index == 31) && i.immediate == 0) {
		// MOV to/from SP is an alias of ADD (imm), #0
		ss << "MOV";
		ss << ' ' << gp_reg_name(i.destination_reg_index, i.is_64bit) << ", " << gp_reg_name(i.source_reg_index, i.is_64bit);
		return ss.str();
	}
	else {
		ss << (i.is_subtraction
			   ? "SUB"
			   : "ADD");
	}

	ss << ' ' << gp_reg_name(i.destination_reg_index, i.is_64bit) << ", " << gp_reg_name(i.source_reg_index, i.is_64bit) << ", #" << std::hex << i.immediate;
	return ss.str();
}

std::string disassembly::to_pretty_string(const InstructionDefs::DataProcImm::FormPcRelAddress &i) {
	std::stringstream ss;

	if (i.rel_to_4kb_page) {
		ss << "ADRP";
	}
	else {
		ss << "ADR";
	}

	ss << ", " << gp_reg_name(i.destination_reg_index) << ", #" << std::hex << i.immediate;
	return ss.str();
}

std::string disassembly::to_pretty_string(const InstructionDefs::DataProcImm::MoveWideImmediate &i) {
	// TODO: Add aliases

	std::stringstream ss;
	ss << "MOV";

	switch (i.op_type) {
		case InstructionDefs::DataProcImm::MoveWideImmediateOpType::Unallocated:
			throw std::runtime_error("Unallocated operation type");
		case InstructionDefs::DataProcImm::MoveWideImmediateOpType::Invert:
			ss << 'N';
			break;
		case InstructionDefs::DataProcImm::MoveWideImmediateOpType::Zero:
			ss << 'Z';
			break;
		case InstructionDefs::DataProcImm::MoveWideImmediateOpType::KeepBits:
			ss << 'K';
			break;
	}

	ss << ' ' << gp_reg_name(i.destination_reg, i.is_64bit) << ", #" << i.immediate;
	if (i.left_shift > 0) {
		ss << "{, LSL #" << i.left_shift << "}";
	}

	return ss.str();
}

namespace {
	template<class T>
	void append_indexing_semantics(std::stringstream& ss, regindex_t base_reg, InstructionDefs::IndexingMode mode, T imm) {
		switch (mode) {
			case InstructionDefs::IndexingMode::PostIndex:
				ss << '[' << gp_reg_name(base_reg) << "], #" << std::hex << std::showbase << imm;
				break;
			case InstructionDefs::IndexingMode::PreIndex:
				ss << '[' << gp_reg_name(base_reg) << ", #" << std::hex << std::showbase << imm << "]!";
				break;
			case InstructionDefs::IndexingMode::UnsignedOffset:
			case InstructionDefs::IndexingMode::SignedOffset:
			case InstructionDefs::IndexingMode::NonTemporalOffset:
				ss << '[' << gp_reg_name(base_reg) << ", #" << std::hex << std::showbase << imm << "]";
				break;
			default:
				throw std::runtime_error("Unsupported indexing mode");
		}
	}
}

std::string disassembly::to_pretty_string(const InstructionDefs::LoadsAndStores::LoadStoreRegUnsignedImm &i) {
	std::stringstream ss;

	if (i.get_is_prefetch()) {
		ss << "#" << std::hex << std::showbase << i.src_dst_reg;
		ss << ", [X" << i.base_reg << ", #" << i.unsigned_imm << "]";
	}
	else {

		ss << (i.is_load ? "LDR" : "STR");

		if (i.is_load && i.is_signed) {
			ss << "S";
		}

		switch (i.size) {
			case 8:
				ss << "B";
				break;
			case 16:
				ss << "H";
				break;
			case 32:
				ss << "W";
				break;
		}
	}

	ss << ' ' << gp_reg_name(i.src_dst_reg) << ", ";

	auto imm = i.indexing_mode == InstructionDefs::IndexingMode::UnsignedOffset
			? i.unsigned_imm
			: i.signed_imm9;
	append_indexing_semantics(ss, i.base_reg, i.indexing_mode, imm);

	return ss.str();
}

std::string disassembly::to_pretty_string(const InstructionDefs::LoadsAndStores::LoadRegisterPair &i) {
	std::stringstream ss;

	if (i.is_load) {
		if (i.encoding == InstructionDefs::IndexingMode::NonTemporalOffset) {
			ss << "LDNP";
		}
		else {
			ss << "LDP";
		}
	}
	else {
		if (i.encoding == InstructionDefs::IndexingMode::NonTemporalOffset) {
			ss << "STNP";
		}
		else {
			ss << "STP";
		}
	}

	ss << ' ' << gp_reg_name(i.first_reg_index) << ", " << gp_reg_name(i.second_reg_index) << ", ";
	append_indexing_semantics(ss, i.base_reg, i.encoding, i.immediate_value);

	return ss.str();
}

std::string disassembly::to_pretty_string(const InstructionDefs::Begsi::Hint &i) {
	switch (i.hintType) {
		case InstructionDefs::Begsi::Hint::Type::Nop:
			return "NOP";
		default:
			throw std::runtime_error("Unknown hint type");
	}
}

std::string disassembly::to_pretty_string(const InstructionDefs::Begsi::UnconditionalBranchImmediate &i) {
	std::stringstream ss;

	ss << (i.is_with_link ? "BL " : "B ");
	insert_signed_hex(ss, i.immediate);

	return ss.str();
}

std::string disassembly::to_pretty_string(const InstructionDefs::Begsi::UnconditionalBranchRegister &i) {
	std::stringstream ss;

	bool addReg = true;
	switch (i.operation_opc) {
		case InstructionDefs::Begsi::UnconditionalBranchRegister::OperationOpc::Normal:
			ss << "BR";
			break;
		case InstructionDefs::Begsi::UnconditionalBranchRegister::OperationOpc::Link:
			ss << "BLR";
			break;
		case InstructionDefs::Begsi::UnconditionalBranchRegister::OperationOpc::Return:
			addReg = i.destination_reg != 30;
			ss << "RET";
			break;
	}
	if (addReg) {
		ss << ' ' << gp_reg_name(i.destination_reg);
	}

	return ss.str();
}

std::string disassembly::to_pretty_string(const InstructionDefs::DataProcReg::LogicalShiftedRegister &i) {
	std::stringstream ss;
	// MOV Xd, Xn alias
	if (i.operation == InstructionDefs::DataProcReg::LogicalShiftedRegister::Operation::Or
		&& i.operand1Reg == 31
		&& i.shiftAmount == 0
		&& i.shiftType == InstructionDefs::DataProcReg::LogicalShiftedRegister::ShiftType::LogicalShiftLeft) {
		ss << "MOV " << gp_reg_name(i.destinationReg) << ", " << gp_reg_name_zero(i.operand2Reg);
	}
	else {
		switch (i.operation) {
			case InstructionDefs::DataProcReg::LogicalShiftedRegister::Operation::And:
				ss << (i.negate ? "BIC" : "AND");
				break;
			case InstructionDefs::DataProcReg::LogicalShiftedRegister::Operation::Or:
				ss << "ORR";
				break;
			case InstructionDefs::DataProcReg::LogicalShiftedRegister::Operation::Xor:
				ss << "EOR";
				break;
			case InstructionDefs::DataProcReg::LogicalShiftedRegister::Operation::AndSetFlags:
				ss << (i.negate ? "BICS" : "ANDS");
				break;
			default:
				throw std::runtime_error("Invalid operation");
		}

		ss << ' ' << gp_reg_name(i.destinationReg, i.is64Bit) << ", "
		   << gp_reg_name_zero(i.operand1Reg, i.is64Bit) << ", " << gp_reg_name_zero(i.operand2Reg, i.is64Bit);

		if (i.shiftAmount > 0) {
			switch (i.shiftType) {
				case InstructionDefs::DataProcReg::LogicalShiftedRegister::ShiftType::LogicalShiftLeft:
					ss << "LSL";
					break;
				case InstructionDefs::DataProcReg::LogicalShiftedRegister::ShiftType::LogicalShiftRight:
					ss << "LSR";
					break;
				case InstructionDefs::DataProcReg::LogicalShiftedRegister::ShiftType::ArythmeticShiftRight:
					ss << "ASR";
					break;
				case InstructionDefs::DataProcReg::LogicalShiftedRegister::ShiftType::RotateRight:
					ss << "ROR";
					break;
				default:
					throw std::runtime_error("Invalid shift type");
			}
			ss << ", #" << i.shiftAmount;
		}
	}

	return ss.str();
}

std::string disassembly::to_pretty_string(const InstructionDefs::Reserved::ReservedCall &i) {
	std::stringstream ss;
	ss << "UDF type: " << std::hex << std::showbase << static_cast<uint32_t>(i.call_type) << ", symbol: #" << i.immediate;
	return ss.str();
}
