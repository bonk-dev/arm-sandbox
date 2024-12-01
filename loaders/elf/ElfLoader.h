#pragma once

#include <string>
#include <vector>
#include <memory>
#include "elf.h"

namespace Loaders {
	class ElfLoader {
	private:
		/**
		 * @var _rawFile
		 * @brief Entire ELF file loaded from disk
		 */
		std::unique_ptr<std::vector<std::byte>> _rawFile;

		/**
		 * @var _filePath
		 * @brief Path to the main ELF executable
		 */
		std::string _filePath;
	public:
		explicit ElfLoader(std::string  executablePath);
		void loadEntireFile();
	};
}
