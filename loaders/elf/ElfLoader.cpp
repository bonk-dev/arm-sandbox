#include "ElfLoader.h"
#include <filesystem>
#include <fstream>
#include <utility>

namespace Loaders {
	ElfLoader::ElfLoader(std::string executablePath) :
		_rawFile{},
		_filePath(std::move(executablePath)) {}

	void ElfLoader::loadEntireFile() {
		std::ifstream f(this->_filePath, std::ios::in | std::ios::binary);

		const auto size = static_cast<std::streamsize>(std::filesystem::file_size(this->_filePath));

		if (!this->_rawFile) {
			this->_rawFile = std::make_unique<std::vector<std::byte>>(size);
		}

		f.read(reinterpret_cast<char*>(this->_rawFile->data()), size);
	}
}