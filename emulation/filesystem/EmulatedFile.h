#pragma once

#include <cstdint>
#include <vector>
#include "File.h"

namespace Filesystem {
	/**
	 * Made up, emulated file
	 */
	class EmulatedFile : public Filesystem::File {
	private:
		size_t _position;
		std::vector<std::byte> _data;
	public:
		/**
		 * Construct an emulated file from a std::string
		 * @param str The string to be put in the file
		 * @param fileId Unique file id
		 */
		explicit EmulatedFile(const std::string& str, uint64_t fileId);

		std::string readString() override;
		void* getUnsafeDataPointer() override;
	};
}
