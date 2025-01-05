#pragma once

#include "../../../emulation/emu_types.h"

namespace InstructionDefs::DataProcImm {
	struct FormPcRelAddress {
	public:
		bool rel_to_4kb_page;

		regindex_t destination_reg_index;
		int32_t immediate : 21;

		explicit FormPcRelAddress(uint32_t encoded);
	};
}
