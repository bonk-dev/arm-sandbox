#pragma once

#include <string>

namespace Filesystem {
	class File {
	private:
		uint64_t _fileId;
	public:
		explicit File(uint64_t fileId) : _fileId(fileId) {}
		virtual ~File() = default;

		[[nodiscard]] uint64_t getFileId() const {
			return this->_fileId;
		}

		[[nodiscard]] virtual std::string readString() = 0;
	};
}
