#include "ElfLoader.h"
#include <filesystem>
#include <fstream>
#include <utility>
#include <iostream>
#include <cstring>
#include <format>

namespace {

}

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

	Elf64_Ehdr ElfLoader::_parseElf64Header() {
		auto* header = reinterpret_cast<Elf64_Ehdr*>(this->_rawFile->data());
		// make a copy
		return *header;
	}

	void ElfLoader::parse() {
		auto elfHeader = this->_parseElf64Header();

		if (strncmp(reinterpret_cast<const char *>(&elfHeader.e_ident[EI_MAG0]), ELFMAG, sizeof(ELFMAG) - 1) != 0) {
			throw std::runtime_error("Not an ELF file");
		}

		switch (elfHeader.e_ident[EI_CLASS]) {
			case ELFCLASS64:
				// supported
				break;
			default:
				throw std::runtime_error(std::format("Unsupported ELF class: {}",
													 elfHeader.e_ident[EI_CLASS]));
		}

		if (elfHeader.e_machine != EM_AARCH64) {
			throw std::runtime_error("Only AARCH64 executables are supported");
		}

		auto sectionHeaders = this->_parseStructs<Elf64_Shdr>(
				elfHeader.e_shnum, elfHeader.e_shoff, elfHeader.e_shentsize);

		Elf64_Shdr* strtabHeader = sectionHeaders[elfHeader.e_shstrndx];
		std::byte* base = this->_rawFile->data();

		char* sectionNameTable = reinterpret_cast<char*>(base + strtabHeader->sh_offset);
		for (Elf64_Shdr* header : sectionHeaders) {
			char* sectionName = &sectionNameTable[header->sh_name];
			std::cout << "Section: " << sectionName << std::endl;
		}

		int a;
	}
}