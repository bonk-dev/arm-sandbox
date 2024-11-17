#include <sstream>
#include "prettify.h"

std::string disassembly::to_pretty_string(AddImmediateInstruction &i) {
	const char reg_prefix = i.is_64bit
							? 'X'
							: 'W';

	std::stringstream ss;
	if (i.set_flags) {
		ss << (i.is_subtraction
			   ? "SUBS"
			   : "ADDS");
	}
	else {
		ss << (i.is_subtraction
			   ? "SUB"
			   : "ADD");
	}

	ss << ", " << reg_prefix << toascii(i.destination_reg_index) << ", #" << std::hex << i.immediate;
	return ss.str();
}

std::string disassembly::to_pretty_string(FormPcRelAddressInstruction &i) {
	std::stringstream ss;

	if (i.rel_to_4kb_page) {
		ss << "ADRP";
	}
	else {
		ss << "ADR";
	}

	ss << ", X" << toascii(i.destination_reg_index) << ", #" << std::hex << i.immediate;
	return ss.str();
}

std::string disassembly::to_pretty_string(LoadRegisterPairInstruction &i) {
	std::stringstream ss;

	if (i.is_load) {
		if (i.encoding == LoadStorePairEncoding::NonTemporalOffset) {
			ss << "LDNP";
		}
		else {
			ss << "LTP";
		}
	}
	else {
		if (i.encoding == LoadStorePairEncoding::NonTemporalOffset) {
			ss << "STNP";
		}
		else {
			ss << "STP";
		}
	}

	const char size = i.is_wide
					  ? 'X'
					  : 'W';
	ss << ' ' << size << toascii(i.first_reg_index) << ", " << size << toascii(i.second_reg_index) << ", ";

	switch (i.encoding) {
		case LoadStorePairEncoding::PostIndex:
			ss << '[' << size << toascii(i.base_reg) << "], #" << i.immediate_value;
			break;
		case LoadStorePairEncoding::PreIndex:
			ss << '[' << size << toascii(i.base_reg) << ", #" << i.immediate_value << "]!";
			break;
		case LoadStorePairEncoding::SignedOffset:
		case LoadStorePairEncoding::NonTemporalOffset:
			ss << '[' << size << toascii(i.base_reg) << "{, #" << i.immediate_value << "}]";
			break;
	}

	return ss.str();
}
