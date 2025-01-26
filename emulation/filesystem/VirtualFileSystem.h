#pragma once

#include <map>
#include <string>
#include <memory>
#include "File.h"
#include "VirtualFileStruct.h"

namespace Filesystem {
	class VirtualFileSystem {
	private:
		uint64_t _availableFileId;
		std::map<std::string, std::shared_ptr<Filesystem::File>> _filesByPath;
		std::map<uint64_t, std::shared_ptr<Filesystem::File>> _filesById;

		/**
		 * Takes the available file id and increments the _availableFileId field;
		 * @return Available file id
		 */
		uint64_t _takeNextFileId();
	public:
		VirtualFileSystem();
		void addFile(const std::string& path, std::shared_ptr<Filesystem::File> file);

		[[nodiscard]] bool doesFileExist(const std::string& path) const;
		[[nodiscard]] Filesystem::File& getFile(const std::string& path);
		[[nodiscard]] Filesystem::File& getFile(const Filesystem::VirtualFileStruct& fileStruct);
	};
}
