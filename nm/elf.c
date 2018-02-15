/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** elf_checking.c
*/
#include <stdio.h>
#include <elf.h>
#include <memory.h>
#include <sys/mman.h>
#include "nm.h"

int check_elf(info_nm_t *info)
{
	Elf64_Ehdr *elf = info->finfo.vadress;
	char header[] = {
		ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3
	};

	if (memcmp(info->finfo.vadress, header, sizeof(header)) != 0)
		return (HEADER_ERROR);
	if (elf->e_ident[EI_CLASS] == ELFCLASSNONE)
		return (nmputerror(info, "File architecture not supported."));
	info->finfo.archi = elf->e_ident[EI_CLASS];
	if(elf->e_ident[EI_DATA] != ELFDATA2LSB)
		return (nmputerror(info, "Unsupported ELF File byte order."));
	if (elf->e_ident[EI_VERSION] != EV_CURRENT)
		return (nmputerror(info, "Unsupported ELF File version."));
	return (0);
}

int print_elf(info_nm_t *info)
{
	if (extract_symbol_list(info))
		return (1);
	print_file(&info->finfo, info->multi_nm);
	munmap(info->finfo.vadress, info->finfo.size);
	return (0);
}