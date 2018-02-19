/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** extrat_symbol.c
*/
#include <stdio.h>
#include <elf.h>
#include "nm.h"

#ifdef ARCHI32
int store_symbol_list_32(info_nm_t *infos, uint32_t size,
	Elf_Sym *symbols, void *strtab_p)
#elif ARCHI64
int store_symbol_list_64(info_nm_t *infos, uint64_t size,
	Elf_Sym *symbols, void *strtab_p)
#endif
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

#ifdef ARCHI32
int create_symbol_list_32(info_nm_t *infos)
#elif ARCHI64
int create_symbol_list_64(info_nm_t *infos)
#endif
{
	#ifdef ARCHI32
	Elf_Shdr *symtab = elf_get_section_32(&infos->finfo, ".symtab");
	#elif ARCHI64
	Elf_Shdr *symtab = elf_get_section_64(&infos->finfo, ".symtab");
	#endif
	Elf_Shdr *strtab;
	uint64_t size;
	Elf_Sym *symbol;
	void *strtab_p;

	if (!symtab || symtab->sh_type != SHT_SYMTAB)
		return (nmputerror(infos, "no symbol."));
	#ifdef ARCHI32
	strtab = elf_get_section_32(&infos->finfo, ".strtab");
	#elif ARCHI64
	strtab = elf_get_section_64(&infos->finfo, ".strtab");
	#endif
	if (!strtab || strtab->sh_type != SHT_STRTAB)
		return (nmputerror(infos, "no strtab."));
	size = (uint64_t) (symtab->sh_size / symtab->sh_entsize);
	symbol = infos->finfo.vadress + symtab->sh_offset;
	strtab_p = infos->finfo.vadress + strtab->sh_offset;
	#ifdef ARCHI32
	if (store_symbol_list_32(infos, (uint32_t) size, symbol, strtab_p))
	#elif ARCHI64
	if (store_symbol_list_64(infos, size, symbol, strtab_p))
	#endif
		return (1);
	return (0);
}

#ifdef ARCHI32
void print_syms_32(info_file_t *info)
#elif ARCHI64
void print_syms_64(info_file_t *info)
#endif
{
	char c;

	for (list_t n = info->sym_links; n != NULL; n = n->next) {
		#ifdef ARCHI32
		c = print_type_32(n->value, info);
		if (((Elf_Sym *)n->value)->st_value ||
		    (c != 'U' && c != 'w'))
			printf("%08x ", ((Elf_Sym *)n->value)->st_value);
		else
			printf("%8s ", "");
		#elif ARCHI64
		c = print_type_64(n->value, info);
		if (((Elf_Sym *)n->value)->st_value ||
		    (c != 'U' && c != 'w'))
			printf("%016lx ", ((Elf_Sym *)n->value)->st_value);
		else
			printf("%16s ", "");
		#endif
		printf("%c ", c);
		printf("%s\n", n->name);
	}
}