/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** extrat_symbol.c
*/
#include <stdio.h>
#include <elf.h>
#include "nm.h"

static int store_symbol_list_64(info_nm_t *infos, uint32_t size,
	Elf64_Sym *symbols, void *strtab_p)
{
	for (size_t i = 1; i < size; i++) {
		if (!is_nm_printable(symbols[i].st_info))
			continue;
		if (!list_add(&infos->finfo.sym_links, &symbols[i])) {
			list_clear(&infos->finfo.sym_links);
			return (nm_perror(infos));
		}
		infos->finfo.sym_links->name = strtab_p + symbols[i].st_name;
	}
	return (0);
}

static int store_symbol_list_32(info_nm_t *infos, uint32_t size,
	Elf32_Sym *symbols, void *strtab_p)
{
	for (size_t i = 1; i < size; i++) {
		if (!is_nm_printable(symbols[i].st_info))
			continue;
		if (!list_add(&infos->finfo.sym_links, &symbols[i])) {
			list_clear(&infos->finfo.sym_links);
			return (nm_perror(infos));
		}
		infos->finfo.sym_links->name = strtab_p + symbols[i].st_name;
	}
	return (0);
}

static int create_symbol_list_64(info_nm_t *infos)
{
	Elf64_Shdr *symtab = elf_get_section_64(&infos->finfo, ".symtab");
	Elf64_Shdr *strtab;
	uint32_t size;
	Elf64_Sym *symbol;
	void *strtab_p;

	if (!symtab || symtab->sh_type != SHT_SYMTAB)
		return (nmputerror(infos, "no symbol."));
	strtab = elf_get_section_64(&infos->finfo, ".strtab");
	if (!strtab || strtab->sh_type != SHT_STRTAB)
		return (nmputerror(infos, "no strtab."));
	size = (uint32_t) (symtab->sh_size / symtab->sh_entsize);
	symbol = infos->finfo.vadress + symtab->sh_offset;
	strtab_p = infos->finfo.vadress + strtab->sh_offset;
	if (store_symbol_list_64(infos, size, symbol, strtab_p))
		return (1);
	return (0);
}

static int create_symbol_list_32(info_nm_t *infos)
{
	Elf32_Shdr *symtab = elf_get_section_32(&infos->finfo, ".symtab");
	Elf32_Shdr *strtab;
	uint32_t size;
	Elf32_Sym *symbol;
	void *strtab_p;

	if (!symtab || symtab->sh_type != SHT_SYMTAB)
		return (nmputerror(infos, "no symbol."));
	strtab = elf_get_section_32(&infos->finfo, ".strtab");
	if (!strtab || strtab->sh_type != SHT_STRTAB)
		return (nmputerror(infos, "no strtab."));
	size = (uint32_t) (symtab->sh_size / symtab->sh_entsize);
	symbol = infos->finfo.vadress + symtab->sh_offset;
	strtab_p = infos->finfo.vadress + strtab->sh_offset;
	if (store_symbol_list_32(infos, size, symbol, strtab_p))
		return (1);
	return (0);
}

int extract_symbol_list(info_nm_t *infos)
{
	if (infos->finfo.archi == ELFCLASS64) {
		if (create_symbol_list_64(infos))
			return (1);
	} else {
		if (create_symbol_list_32(infos))
			return (1);
	}
	list_sort(infos->finfo.sym_links);
	return (0);
}