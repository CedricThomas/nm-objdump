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

#ifdef ARCHI32
static char get_bind_symbol_32(Elf_Sym *sym)
#elif ARCHI64
static char get_bind_symbol_64(Elf_Sym *sym)
#endif
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


#ifdef ARCHI32
static char get_progbits_32(Elf_Sym *sym, Elf_Shdr *shdr)
#elif ARCHI64
static char get_progbits_64(Elf_Sym *sym, Elf_Shdr *shdr)
#endif
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

#ifdef ARCHI32
static char get_special_32(Elf_Sym *sym, Elf_Shdr *shdr)
#elif ARCHI64
static char get_special_64(Elf_Sym *sym, Elf_Shdr *shdr)
#endif
{
	char c = '?';

	if (shdr[sym->st_shndx].sh_type == SHT_NOBITS &&
	    shdr[sym->st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'B';
	else if (shdr[sym->st_shndx].sh_type == SHT_DYNAMIC)
		c = 'D';
	return (c);
}

#ifdef ARCHI32
static char get_basic_32(Elf_Sym *sym)
#elif ARCHI64
static char get_basic_64(Elf_Sym *sym)
#endif
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

#ifdef ARCHI32
char print_type_32(Elf_Sym *sym, info_file_t *info)
#elif ARCHI64
char print_type_64(Elf_Sym *sym, info_file_t *info)
#endif
{
	#ifdef ARCHI32
	Elf_Shdr *shdr = elf_get_sheader_32(info->vadress);
	char c = get_bind_symbol_32(sym);

	if (c == '?')
		c = get_basic_32(sym);
	if (c == '?')
		c = get_progbits_32(sym, shdr);
	if (c == '?')
		c = get_special_32(sym, shdr);
	#elif ARCHI64
	Elf_Shdr *shdr = elf_get_sheader_64(info->vadress);
	char c = get_bind_symbol_64(sym);

	if (c == '?')
		c = get_basic_64(sym);
	if (c == '?')
		c = get_progbits_64(sym, shdr);
	if (c == '?')
		c = get_special_64(sym, shdr);
	#endif
	if (ELF64_ST_BIND(sym->st_info) == STB_LOCAL && c != '?')
		c += 32;
	return c;
}
