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

		template<class T>
		std::vector<T*> _parseStructs(Elf64_Half count, Elf64_Off fileOffset, Elf64_Half size) {
			if (fileOffset <= 0 || size <= 0 || count <= 0) {
				return {};
			}

			std::vector<T*> headers = std::vector<T*>(count);
			std::byte* data = this->_rawFile->data();

			for (int i = 0; i < count; ++i) {
				const Elf64_Off dataOffset = fileOffset + i * size;
				if (dataOffset >= this->_rawFile->size()) {
					throw std::runtime_error("Header was outside of the ELF file");
				}

				std::byte* sectionPtr = (data + dataOffset);
				auto* headerPtr = reinterpret_cast<T*>(sectionPtr);
				headers[i] = (headerPtr);
			}

			return headers;
		}

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
