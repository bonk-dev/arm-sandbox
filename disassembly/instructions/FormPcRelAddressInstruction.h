#pragma once

#include "../../emulation/emu_types.h"

typedef struct FormPcRelAddressInstruction {
private:
	static int32_t decode_immediate(uint32_t encoded);
public:
	bool rel_to_4kb_page;

	regindex_t destination_reg_index;
	int32_t immediate;

	explicit FormPcRelAddressInstruction(uint32_t encoded);
} FormPcRelAddressInstruction;
