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

		[[nodiscard]] Elf64_Ehdr _parseElf64Header();
	public:
		explicit ElfLoader(std::string  executablePath);
		void loadEntireFile();

		/***
		 * @brief This method will parse the ELF file and return abstracted info about the executable
		 */
		void parse();
	};
}
