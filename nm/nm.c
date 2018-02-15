/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** nm.c
*/
#include <stdio.h>
#include <elf.h>
#include <sys/mman.h>
#include <memory.h>
#include <ar.h>
#include <stdlib.h>
#include <zconf.h>
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
//	else if (shdr[sym->st_shndx].sh_type == )
//	{
//		c = 'r';
//	}
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

static void print_syms(info_file_t *info)
{
	void *sheader = elf_get_sheader(info->vadress);
	char c;

	for (list_t n = info->sym_links; n != NULL; n = n->next) {
		c = print_type(n->value, sheader);
		if (((Elf64_Sym *)n->value)->st_value || (c != 'U' && c != 'w'))
			printf("%016lx ", ((Elf64_Sym *)n->value)->st_value);
		else
			printf("%16s ", "");
		printf("%c ", c);
		printf("%s\n", n->name);
	}
}

int print_file(info_file_t *info, int multi)
{
	if (multi)
		printf("\n%s:\n", info->name);
	print_syms(info);
	list_clear(&info->sym_links);
	return (0);
}

int nm(info_nm_t *info)
{
	int format = 0;

	if (map_file(&info->finfo))
		return (nm_perror(info));
	format = check_elf(info);
	if (format == SUCCESS) {
		if (print_elf(info))
			return (1);
	} else if (check_ar(info) == SUCCESS) {
		if (print_ar(info))
			return (1);
	} else
		return (nmputerror(info, "Invalid elf header."));
	return (0);
}