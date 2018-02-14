/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** nm.c
*/
#include <stdio.h>
#include <elf.h>
#include "nm.h"

char print_type(Elf64_Sym *sym, Elf64_Shdr *shdr)
{
	char c;

	if (ELF64_ST_BIND(sym->st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF64_ST_BIND(sym->st_info) == STB_WEAK)
	{
		c = 'W';
		if (sym->st_shndx == SHN_UNDEF)
			c = 'w';
	}
	else if (ELF64_ST_BIND(sym->st_info) == STB_WEAK && ELF64_ST_TYPE(sym->st_info) == STT_OBJECT)
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
	if (ELF64_ST_BIND(sym->st_info) == STB_LOCAL && c != '?')
		c += 32;
	return c;
}

static void print_syms(info_nm_t *info)
{
	void *sheader = elf_get_sheader(info->finfo.vadress);

	for (list_t n = info->finfo.sym_links; n != NULL; n = n->next) {
		if (((Elf64_Sym *)n->value)->st_value)
			printf("%016lx ", ((Elf64_Sym *)n->value)->st_value);
		else
			printf("%16s ", "");
		printf("%c ", print_type(n->value, sheader));
		printf("%s\n", n->name);
	}
}

int nm(info_nm_t *infos)
{
	if (extract_symbol_list(infos))
		return (1);
	if (infos->multi_nm)
		printf("\n%s:\n", infos->finfo.name);
	print_syms(infos);
	list_clear(&(infos->finfo.sym_links));
	return (0);
}