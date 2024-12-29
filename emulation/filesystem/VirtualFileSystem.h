#pragma once

#include <map>
#include <string>
#include <memory>
#include "File.h"

namespace Filesystem {
	class VirtualFileSystem {
	private:
		uint64_t _availableFileId;
		std::map<std::string, std::unique_ptr<Filesystem::File>> _files;

		/**
		 * Takes the available file id and increments the _availableFileId field;
		 * @return Available file id
		 */
		uint64_t _takeNextFileId();
	public:
		VirtualFileSystem();
		void addFile(const std::string& path, std::unique_ptr<Filesystem::File> file);

		[[nodiscard]] bool doesFileExist(const std::string& path) const;
		[[nodiscard]] Filesystem::File& getFile(const std::string& path);
	};
}
