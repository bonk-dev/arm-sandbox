// This file contains needed definitions from elf.h
// elf.h is not available on Windows, so we need to supply it ourselves

#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "bugprone-reserved-identifier"
#include <cstdint>

typedef uint64_t __u64;
typedef int64_t __s64;
typedef uint16_t __u16;
typedef int16_t	__s16;
typedef int32_t __s32;
typedef uint32_t __u32;

/* 64-bit ELF base types. */
typedef __u64	Elf64_Addr;
typedef __u16	Elf64_Half;
typedef __s16	Elf64_SHalf;
typedef __u64	Elf64_Off;
typedef __s32	Elf64_Sword;
typedef __u32	Elf64_Word;
typedef __u64	Elf64_Xword;
typedef __s64	Elf64_Sxword;

/* These constants are for the segment types stored in the image headers */
#define PT_NULL    0
#define PT_LOAD    1
#define PT_DYNAMIC 2
#define PT_INTERP  3
#define PT_NOTE    4
#define PT_SHLIB   5
#define PT_PHDR    6
#define PT_LOOS    0x60000000      /* OS-specific */
#define PT_HIOS    0x6fffffff      /* OS-specific */
#define PT_LOPROC  0x70000000
#define PT_HIPROC  0x7fffffff
#define PT_GNU_STACK	(PT_LOOS + 0x474e551)

#define EI_NIDENT	16

struct Elf64_Ehdr {
	unsigned char	e_ident[EI_NIDENT];	/* ELF "magic number" */
	Elf64_Half e_type;
	Elf64_Half e_machine;
	Elf64_Word e_version;
	Elf64_Addr e_entry;		/* Entry point virtual address */
	Elf64_Off e_phoff;		/* Program header table file offset */
	Elf64_Off e_shoff;		/* Section header table file offset */
	Elf64_Word e_flags;
	Elf64_Half e_ehsize;
	Elf64_Half e_phentsize;
	Elf64_Half e_phnum;
	Elf64_Half e_shentsize;
	Elf64_Half e_shnum;
	Elf64_Half e_shstrndx;
};

struct Elf64_Phdr {
	Elf64_Word p_type;
	Elf64_Word p_flags;
	Elf64_Off p_offset;		/* Segment file offset */
	Elf64_Addr p_vaddr;		/* Segment virtual address */
	Elf64_Addr p_paddr;		/* Segment physical address */
	Elf64_Xword p_filesz;		/* Segment size in file */
	Elf64_Xword p_memsz;		/* Segment size in memory */
	Elf64_Xword p_align;		/* Segment alignment, file & memory */
};

struct Elf64_Shdr {
	Elf64_Word sh_name;		/* Section name, index in string tbl */
	Elf64_Word sh_type;		/* Type of section */
	Elf64_Xword sh_flags;		/* Miscellaneous section attributes */
	Elf64_Addr sh_addr;		/* Section virtual addr at execution */
	Elf64_Off sh_offset;		/* Section file offset */
	Elf64_Xword sh_size;		/* Size of section in bytes */
	Elf64_Word sh_link;		/* Index of another section */
	Elf64_Word sh_info;		/* Additional section information */
	Elf64_Xword sh_addralign;	/* Section alignment */
	Elf64_Xword sh_entsize;	/* Entry size if section holds table */
};

struct Elf64_Dyn {
	Elf64_Sxword d_tag;		/* entry tag value */
	union {
		Elf64_Xword d_val;
		Elf64_Addr d_ptr;
	} d_un;
};

/* The following are used with relocations */
#define ELF64_R_SYM(i)			((i) >> 32)
#define ELF64_R_TYPE(i)			((i) & 0xffffffff)
#define R_AARCH64_JUMP_SLOT    1026	/* Create PLT entry.  */

struct Elf64_Rela {
	Elf64_Addr r_offset;	/* Location at which to apply the action */
	Elf64_Xword r_info;	/* index and type of relocation */
	Elf64_Sxword r_addend;	/* Constant addend used to compute value */
};

struct Elf64_Sym {
	Elf64_Word st_name;		/* Symbol name, index in string tbl */
	unsigned char	st_info;	/* Type and binding attributes */
	unsigned char	st_other;	/* No defined meaning, 0 */
	Elf64_Half st_shndx;		/* Associated section index */
	Elf64_Addr st_value;		/* Value of the symbol */
	Elf64_Xword st_size;		/* Associated symbol size */
};

#define	EI_MAG0		0		/* e_ident[] indexes */
#define	EI_MAG1		1
#define	EI_MAG2		2
#define	EI_MAG3		3
#define	EI_CLASS	4
#define	EI_DATA		5
#define	EI_VERSION	6
#define	EI_OSABI	7
#define	EI_PAD		8

#define	ELFMAG0		0x7f		/* EI_MAG */
#define	ELFMAG1		'E'
#define	ELFMAG2		'L'
#define	ELFMAG3		'F'
#define	ELFMAG		"\177ELF"
#define	SELFMAG		4

#define	ELFCLASSNONE	0		/* EI_CLASS */
#define	ELFCLASS32	1
#define	ELFCLASS64	2
#define	ELFCLASSNUM	3

#define EM_AARCH64	183	/* ARM AARCH64 */


#define DT_NULL		0
#define DT_NEEDED	1
#define DT_PLTRELSZ	2


/* sh_type */
#define SHT_NULL	0
#define SHT_PROGBITS	1
#define SHT_SYMTAB	2
#define SHT_STRTAB	3
#define SHT_RELA	4
#define SHT_HASH	5
#define SHT_DYNAMIC	6
#define SHT_NOTE	7
#define SHT_NOBITS	8
#define SHT_REL		9
#define SHT_SHLIB	10
#define SHT_DYNSYM	11
#define SHT_NUM		12
#define SHT_LOPROC	0x70000000
#define SHT_HIPROC	0x7fffffff
#define SHT_LOUSER	0x80000000
#define SHT_HIUSER	0xffffffff

/* sh_flags */
#define SHF_WRITE		0x1
#define SHF_ALLOC		0x2
#define SHF_EXECINSTR		0x4
#define SHF_RELA_LIVEPATCH	0x00100000
#define SHF_RO_AFTER_INIT	0x00200000
#define SHF_MASKPROC		0xf0000000

/* special section indexes */
#define SHN_UNDEF	0
#define SHN_LORESERVE	0xff00
#define SHN_LOPROC	0xff00
#define SHN_HIPROC	0xff1f
#define SHN_LIVEPATCH	0xff20
#define SHN_ABS		0xfff1
#define SHN_COMMON	0xfff2
#define SHN_HIRESERVE	0xffff
#pragma clang diagnostic pop