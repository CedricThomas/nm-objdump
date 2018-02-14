/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** extrat_symbol.c
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <elf.h>
#include "nm.h"

static int map_file(info_file_t *finfo)
{
	struct stat s;
	int fd;

	fd = open(finfo->name, O_RDONLY);
	if (fd == -1)
		return (1);
	if (fstat(fd, &s) == -1)
		return (1);
	finfo->size = (size_t) s.st_size;
	finfo->vadress = mmap(0, finfo->size, PROT_READ,
	                      MAP_PRIVATE, fd, 0);
	if (finfo->vadress == MAP_FAILED)
		return (1);
	close(fd);
	return (0);
}

static int store_symbol_list(info_nm_t *infos, uint32_t size,
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

static int create_symbol_list(info_nm_t *infos)
{
	Elf64_Shdr *symtab = elf_get_section(infos, ".symtab");
	Elf64_Shdr *strtab;
	uint32_t size;
	Elf64_Sym *symbol;
	void *strtab_p;

	if (!symtab || symtab->sh_type != SHT_SYMTAB)
		return (nmputerror(infos, "no symbol."));
	strtab = elf_get_section(infos, ".strtab");
	if (!strtab || strtab->sh_type != SHT_STRTAB)
		return (nmputerror(infos, "no strtab."));
	size = (uint32_t) (symtab->sh_size / symtab->sh_entsize);
	symbol = infos->finfo.vadress + symtab->sh_offset;
	strtab_p = infos->finfo.vadress + strtab->sh_offset;
	if (store_symbol_list(infos, size, symbol, strtab_p))
		return (1);
	return (0);
}

int extract_symbol_list(info_nm_t *infos)
{
	if (map_file(&infos->finfo))
		return (nm_perror(infos));
	if (check_elf(infos)) // archi
		return (1);
	if (create_symbol_list(infos)) // archi
		return (1);
	list_sort(infos->finfo.sym_links);
	return (0);
}