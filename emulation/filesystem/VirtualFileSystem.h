#pragma once

#include <map>
#include <string>
#include <memory>
#include "File.h"

namespace Filesystem {
	class VirtualFileSystem {
	private:
		std::map<std::string, std::unique_ptr<Filesystem::File>> _files;
	public:
		VirtualFileSystem();
		void addFile(const std::string& path, std::unique_ptr<Filesystem::File> file);

		[[nodiscard]] bool doesFileExist(const std::string& path) const;
		[[nodiscard]] Filesystem::File& getFile(const std::string& path);
	};
}
