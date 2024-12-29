#pragma once

#include <cstdint>

namespace Filesystem {
/**
 * @brief Used in the virtual memory as the FILE struct
 * @see https://en.cppreference.com/w/c/io/FILE
 */
	typedef struct VirtualFileStruct {
		uint64_t file_id;

		[[nodiscard]] bool isValid() const {
			return this->file_id > 0;
		}
	} VirtualFileStruct;
}
