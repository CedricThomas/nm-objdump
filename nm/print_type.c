/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** print_type.c
*/
#include <stdlib.h>
#include <elf.h>
#include <memory.h>
#include "nm.h"

static char get_bind_symbol_64(Elf64_Sym *sym)
{
	char c = '?';

	if (ELF64_ST_BIND(sym->st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF64_ST_BIND(sym->st_info) == STB_WEAK) {
		c = 'W';
		if (sym->st_shndx == SHN_UNDEF)
			c = 'w';
	} else if (ELF64_ST_BIND(sym->st_info) == STB_WEAK &&
	           ELF64_ST_TYPE(sym->st_info) == STT_OBJECT) {
		c = 'V';
		if (sym->st_shndx == SHN_UNDEF)
			c = 'v';
	}
	return (c);
}

static char get_progbits_64(Elf64_Sym *sym, Elf64_Shdr *shdr)
{
	char c = '?';

	if (shdr[sym->st_shndx].sh_type == SHT_PROGBITS
	         && shdr[sym->st_shndx].sh_flags == SHF_ALLOC)
		c = 'R';
	else if (shdr[sym->st_shndx].sh_type == SHT_PROGBITS &&
	         shdr[sym->st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'D';
	else if (shdr[sym->st_shndx].sh_type == SHT_PROGBITS &&
	         shdr[sym->st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		c = 'T';
	return (c);
}

static char get_special_64(Elf64_Sym *sym, Elf64_Shdr *shdr)
{
	char c = '?';

	if (shdr[sym->st_shndx].sh_type == SHT_NOBITS &&
	    shdr[sym->st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'B';
	else if (shdr[sym->st_shndx].sh_type == SHT_DYNAMIC)
		c = 'D';
	return (c);
}

static char get_basic_64(Elf64_Sym *sym)
{
	char c;

	switch (sym->st_shndx) {
		case SHN_UNDEF:
			c = 'U';
			break;
		case SHN_ABS:
			c = 'A';
			break;
		case SHN_COMMON:
			c = 'C';
			break;
		default:
			c = '?';
			break;
	}
	return (c);
}

char print_type_64(Elf64_Sym *sym, info_file_t *info)
{
	Elf64_Shdr *shdr = elf_get_sheader_64(info->vadress);
	char c = get_bind_symbol_64(sym);

	if (c == '?')
		c = get_basic_64(sym);
	if (c == '?')
		c = get_progbits_64(sym, shdr);
	if (c == '?')
		c = get_special_64(sym, shdr);
	if (ELF64_ST_BIND(sym->st_info) == STB_LOCAL && c != '?')
		c += 32;
	return c;
}

char print_type_32(Elf32_Sym *sym, info_file_t *info)
{
	Elf32_Shdr *shdr = elf_get_sheader_32(info->vadress);
	char c;

	if (ELF32_ST_BIND(sym->st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF32_ST_BIND(sym->st_info) == STB_WEAK)
	{
		c = 'W';
		if (sym->st_shndx == SHN_UNDEF)
			c = 'w';
	}
	else if (ELF32_ST_BIND(sym->st_info) == STB_WEAK &&
		ELF32_ST_TYPE(sym->st_info) == STT_OBJECT)
	{
		c = 'V';
		if (sym->st_shndx == SHN_UNDEF)
			c = 'v';
	}
	else if (sym->st_shndx == SHN_UNDEF)
		c = 'U';
	else if (sym->st_shndx == SHN_ABS)
		c = 'A';
	else if (sym->st_shndx == SHN_COMMON)
		c = 'C';
	else if (shdr[sym->st_shndx].sh_type == SHT_NOBITS
	         && shdr[sym->st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'B';
	else if (shdr[sym->st_shndx].sh_type == SHT_PROGBITS
	         && shdr[sym->st_shndx].sh_flags == SHF_ALLOC)
		c = 'R';
	else if (shdr[sym->st_shndx].sh_type == SHT_PROGBITS
	         && shdr[sym->st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'D';
	else if (shdr[sym->st_shndx].sh_type == SHT_PROGBITS
	         && shdr[sym->st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		c = 'T';
	else if (shdr[sym->st_shndx].sh_type == SHT_DYNAMIC)
		c = 'D';
	else
		c = '?';
	if (ELF32_ST_BIND(sym->st_info) == STB_LOCAL && c != '?')
		c += 32;
	return c;
}
