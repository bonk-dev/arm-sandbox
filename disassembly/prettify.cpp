#include <sstream>
#include <format>
#include "prettify.h"

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

std::string disassembly::to_pretty_string(InstructionDefs::AddImmediateInstruction &i) {
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

std::string disassembly::to_pretty_string(InstructionDefs::FormPcRelAddressInstruction &i) {
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

std::string disassembly::to_pretty_string(InstructionDefs::MoveWideImmediateInstruction &i) {
	// TODO: Add aliases

	std::stringstream ss;
	ss << "MOV";

	switch (i.op_type) {
		case InstructionDefs::MoveWideImmediateInstructionOpType::Unallocated:
			throw std::runtime_error("Unallocated operation type");
		case InstructionDefs::MoveWideImmediateInstructionOpType::Invert:
			ss << 'N';
			break;
		case InstructionDefs::MoveWideImmediateInstructionOpType::Zero:
			ss << 'Z';
			break;
		case InstructionDefs::MoveWideImmediateInstructionOpType::KeepBits:
			ss << 'K';
			break;
	}

	ss << ' ' << gp_reg_name(i.destination_reg, i.is_64bit) << ", #" << i.immediate;
	if (i.left_shift > 0) {
		ss << "{, LSL #" << i.left_shift << "}";
	}

	return ss.str();
}

std::string disassembly::to_pretty_string(InstructionDefs::LoadRegisterPairInstruction &i) {
	std::stringstream ss;

	if (i.is_load) {
		if (i.encoding == InstructionDefs::LoadStorePairEncoding::NonTemporalOffset) {
			ss << "LDNP";
		}
		else {
			ss << "LDP";
		}
	}
	else {
		if (i.encoding == InstructionDefs::LoadStorePairEncoding::NonTemporalOffset) {
			ss << "STNP";
		}
		else {
			ss << "STP";
		}
	}

	ss << ' ' << gp_reg_name(i.first_reg_index) << ", " << gp_reg_name(i.second_reg_index) << ", ";

	switch (i.encoding) {
		case InstructionDefs::LoadStorePairEncoding::PostIndex:
			ss << '[' << gp_reg_name(i.base_reg) << "], #" << i.immediate_value;
			break;
		case InstructionDefs::LoadStorePairEncoding::PreIndex:
			ss << '[' << gp_reg_name(i.base_reg) << ", #" << i.immediate_value << "]!";
			break;
		case InstructionDefs::LoadStorePairEncoding::SignedOffset:
		case InstructionDefs::LoadStorePairEncoding::NonTemporalOffset:
			ss << '[' << gp_reg_name(i.base_reg) << "{, #" << i.immediate_value << "}]";
			break;
	}

	return ss.str();
}

std::string disassembly::to_pretty_string(InstructionDefs::UnconditionalBranchImmediateInstruction &i) {
	std::stringstream ss;

	ss << (i.is_with_link ? "BL " : "B ");
	insert_signed_hex(ss, i.immediate);

	return ss.str();
}
