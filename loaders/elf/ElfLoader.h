#pragma once

#include <string>
#include <vector>
#include <memory>
#include "elf-multiplatform.h"
#include "../../emulation/CpuVirtualMemory.h"
#include "../../emulation/libraries/Mapper.h"
#include "../../logging/LoggerBase.h"

namespace Loaders {
	class ElfLoader {
	private:
		/**
		 * @brief Logger
		 */
		std::unique_ptr<Logging::LoggerBase> _logger;

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

		/**
		 * @var _elfSectionHeaders
		 * @brief Lazily allocated std::vector of ELF64 section headers std::vector
		 */
		std::unique_ptr<std::vector<Elf64_Shdr*>> _elfSectionHeaders;

		/**
		 * @brief Address of the first instruction which should be executed
		 */
		virtual_address_t _entryPoint;

		size_t _relaPltOffset;
		size_t _dynStrOffset;
		size_t _dynSymOffset;
		unsigned long _pltRelocs;

		template<class T>
		std::unique_ptr<std::vector<T*>> _parseStructs(Elf64_Half count, Elf64_Off fileOffset, Elf64_Half size) {
			if (fileOffset <= 0 || size <= 0 || count <= 0) {
				return {};
			}

			auto headers = std::make_unique<std::vector<T*>>(count);
			std::byte* data = this->_rawFile->data();

			for (int i = 0; i < count; ++i) {
				const Elf64_Off dataOffset = fileOffset + i * size;
				if (dataOffset >= this->_rawFile->size()) {
					throw std::runtime_error("Header was outside of the ELF file");
				}

				std::byte* sectionPtr = (data + dataOffset);
				auto* headerPtr = reinterpret_cast<T*>(sectionPtr);
				(*headers)[i] = (headerPtr);
			}

			return headers;
		}

		[[nodiscard]] Elf64_Ehdr* _parseElf64Header();
		[[nodiscard]] char const* _getSectionName(Elf64_Shdr *sectionHeader);
		[[nodiscard]] char const* _getSectionName(Elf64_Word index);
	public:
		explicit ElfLoader(std::string  executablePath);
		void loadEntireFile();

		/**
		 * @brief This method will parse the ELF file and return abstracted info about the executable
		 */
		void parse();

		/**
		 * Get the ELF entry point
		 * @return The entry point found in the ELF header
		 */
		[[nodiscard]] virtual_address_t getEntryPoint() const;

		/**
		 * @brief Allocates appropriate sections in the virtual memory
		 */
		void allocateSections(CpuVirtualMemory& memory);

		void linkSymbols(Emulation::Libraries::Mapper& mapper, CpuVirtualMemory& memory);
	};
}
