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
		 */
		explicit EmulatedFile(const std::string& str);

		std::string readString() override;
		void* getUnsafeDataPointer() override;
	};
}
