#include "ElfLoader.h"
#include <filesystem>
#include <fstream>
#include <utility>
#include <iostream>
#include <cstring>
#include <format>

#include "../../logging/LoggerFactory.h"

namespace {
	constexpr bool has_flag_set(Elf64_Word value, Elf64_Word mask) {
		return (value & mask) == mask;
	}
}

namespace Loaders {
	ElfLoader::ElfLoader(std::string executablePath) :
		_logger(Logging::createLogger("ElfLoader")),
		_rawFile{},
		_filePath(std::move(executablePath)),
		_entryPoint(0),
		_relaPltOffset(0),
		_dynStrOffset(0),
		_dynSymOffset(0),
		_pltRelocs(0) {}

	void ElfLoader::loadEntireFile() {
		std::ifstream f(this->_filePath, std::ios::in | std::ios::binary);

		const auto size = static_cast<std::streamsize>(std::filesystem::file_size(this->_filePath));

		if (!this->_rawFile) {
			this->_rawFile = std::make_unique<std::vector<std::byte>>(size);
		}

		f.read(reinterpret_cast<char*>(this->_rawFile->data()), size);
	}

	Elf64_Ehdr* ElfLoader::_parseElf64Header() {
		return reinterpret_cast<Elf64_Ehdr*>(this->_rawFile->data());
	}

	void ElfLoader::parse() {
		auto elfHeader = this->_parseElf64Header();
		this->_entryPoint = elfHeader->e_entry;

		if (strncmp(reinterpret_cast<const char *>(&elfHeader->e_ident[EI_MAG0]), ELFMAG, sizeof(ELFMAG) - 1) != 0) {
			throw std::runtime_error("Not an ELF file");
		}

		switch (elfHeader->e_ident[EI_CLASS]) {
			case ELFCLASS64:
				// supported
				break;
			default:
				throw std::runtime_error(std::format("Unsupported ELF class: {}",
													 elfHeader->e_ident[EI_CLASS]));
		}

		if (elfHeader->e_machine != EM_AARCH64) {
			throw std::runtime_error("Only AARCH64 executables are supported");
		}

		this->_elfSectionHeaders = this->_parseStructs<Elf64_Shdr>(
				elfHeader->e_shnum, elfHeader->e_shoff, elfHeader->e_shentsize);

		for (Elf64_Shdr* header : *this->_elfSectionHeaders) {
			const char* sectionName = this->_getSectionName(header);
			_logger->verbose() << "Section: " << sectionName << std::endl;

			if (strcmp(sectionName, ".dynamic") == 0) {
				auto* dynEntry = reinterpret_cast<Elf64_Dyn*>(this->_rawFile->data() + header->sh_offset);
				while (dynEntry->d_tag != DT_NULL) {
					if (dynEntry->d_tag == DT_PLTRELSZ) {
						_pltRelocs = dynEntry->d_un.d_val / sizeof(Elf64_Rela);
						_logger->verbose() << "Amount of plt relocs: " << _pltRelocs << std::endl;
					}
					dynEntry++;
				}
			}
			else if (strcmp(sectionName, ".rela.plt") == 0) {
				_relaPltOffset = header->sh_offset;
			}
			else if (strcmp(sectionName, ".dynstr") == 0) {
				_dynStrOffset = header->sh_offset;
			}
			else if (strcmp(sectionName, ".dynsym") == 0) {
				_dynSymOffset = header->sh_offset;
			}
		}
	}

	virtual_address_t ElfLoader::getEntryPoint() const {
		return this->_entryPoint;
	}

	void ElfLoader::allocateSections(CpuVirtualMemory& memory) {
		if (!this->_elfSectionHeaders) {
			throw std::runtime_error("Elf section headers were not initalized");
		}

		// skip the inital entry
		for (int i = 1; i < this->_elfSectionHeaders->size(); ++i) {
			if (i >= SHN_LORESERVE && i <= SHN_HIRESERVE) {
				continue;
			}

			Elf64_Shdr* header = this->_elfSectionHeaders->at(i);
			if (has_flag_set(header->sh_flags, SHF_ALLOC) && header->sh_type != SHT_NOBITS) {
				_logger->verbose() << "Allocating " << this->_getSectionName(header)
					<< " in virtual memory at virtual: " << std::hex << std::showbase << header->sh_addr  << std::endl;
				if (header->sh_addr == 0) {
					throw std::runtime_error("ELF section has the SHF_ALLOC flag set, but the sh_addr field is 0");
				}

				memory.allocateSegment(header->sh_addr, header->sh_size);
				memory.write(
						header->sh_addr,
						this->_rawFile->begin() + header->sh_offset,
						header->sh_size);
			}
			else {
				_logger->verbose() << "Not allocating " << this->_getSectionName(header) << std::endl;
			}
		}
	}

	void ElfLoader::linkSymbols(Emulation::Libraries::Mapper& mapper, CpuVirtualMemory& memory) {
		if (_dynStrOffset == 0 || _dynSymOffset == 0 || _relaPltOffset == 0) {
			throw std::runtime_error("Invalid ELF section offsets");
		}

		const auto relaPtr = reinterpret_cast<Elf64_Rela*>(_rawFile->data() + _relaPltOffset);
		const auto dynsym = reinterpret_cast<Elf64_Sym*>(_rawFile->data() + _dynSymOffset);
		const auto dynstr = reinterpret_cast<const char*>(_rawFile->data() + _dynStrOffset);

		_logger->verbose() << "Scanning .rela.plt" << std::endl;
		for (int i = 0; i < _pltRelocs; ++i) {
			const Elf64_Rela* rela = relaPtr + i;

			if (const auto type = ELF64_R_TYPE(rela->r_info); type != R_AARCH64_JUMP_SLOT) {
				continue;
			}

			const auto symbolIndex = ELF64_R_SYM(rela->r_info);
			const Elf64_Sym* symbolInfo = dynsym + symbolIndex;
			const char* symbolName = &dynstr[symbolInfo->st_name];

			_logger->verbose() << "R_AARCH64_JUMP_SLOT symbol name: " << symbolName << std::endl;
			_logger->verbose() << "R_AARCH64_JUMP_SLOT slot offset: " << rela->r_offset << std::endl;
			_logger->verbose() << "Trying to link with the Mapper" << std::endl;
			const virtual_address_t jumpAddress = mapper.mapLibraryImplementation(symbolName, memory);
			_logger->verbose() << "Jump address: " << jumpAddress << std::endl;

			_logger->verbose() << "Updating the PLT GOT" << std::endl;
			memory.write(rela->r_offset, static_cast<uint32_t>(jumpAddress));
		}
	}

	char const *ElfLoader::_getSectionName(Elf64_Shdr *const sectionHeader) {
		return this->_getSectionName(sectionHeader->sh_name);
	}

	char const *ElfLoader::_getSectionName(Elf64_Word index) {
		std::byte* base = this->_rawFile->data();
		Elf64_Ehdr* elfHeader = this->_parseElf64Header();
		Elf64_Shdr* strtabHeader = (*this->_elfSectionHeaders)[elfHeader->e_shstrndx];

		char* sectionNameTable = reinterpret_cast<char*>(base + strtabHeader->sh_offset);
		return &sectionNameTable[index];
	}
}