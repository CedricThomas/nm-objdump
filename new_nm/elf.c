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

static inline size_t eheader_size(size_t archi)
{
	if (archi == ELFCLASS32)
		return (sizeof(Elf32_Ehdr));
	else
		return (sizeof(Elf64_Ehdr));
}

static int check_magic_nbr(info_nm_t *info, Elf64_Ehdr *elf)
{
	char header[] = {
		ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3
	};

	if (info->finfo.size < sizeof(header))
		return (nmputerror(info, "File truncated"));
	else if (memcmp(info->finfo.vadress, header, sizeof(header)) != 0)
		return (nmputerror(info, "File format not recognized"));
	else if (info->finfo.size < sizeof(elf->e_ident))
		return (nmputerror(info, "File format not recognized"));
	return (SUCCESS);
}

static int check_basic_infos(info_nm_t *info, Elf64_Ehdr *elf)
{
	if (elf->e_ident[EI_CLASS] == ELFCLASSNONE)
		return (nmputerror(info, "File architecture not supported"));
	info->finfo.archi = elf->e_ident[EI_CLASS];
	if(elf->e_ident[EI_DATA] != ELFDATA2LSB)
		return (nmputerror(info, "File byte order not supported"));
	else if (elf->e_ident[EI_VERSION] != EV_CURRENT)
		return (nmputerror(info, "File ELF version not supported"));
	return (SUCCESS);
}

int check_elf(info_nm_t *info)
{
	Elf64_Ehdr *elf = info->finfo.vadress;

	if (check_magic_nbr(info, elf) == ERROR ||
		check_basic_infos(info, elf) == ERROR)
		return (ERROR);
	else if (info->finfo.size < eheader_size(info->finfo.archi))
		return (nmputerror(info, "File format not recognized"));
	else {
		if (info->finfo.archi == ELFCLASS32 &&
			info->finfo.size < ((Elf32_Ehdr *) elf)->e_shoff)
			return (nmputerror(info, "File truncated"));
		else if (info->finfo.archi == ELFCLASS64 &&
			info->finfo.size < elf->e_shoff)
			return (nmputerror(info, "File truncated"));
	}
	return (SUCCESS);
}

int print_elf(info_nm_t *info)
{
	if (extract_symbol_list(info))
		return (ERROR);
	print_file(&info->finfo, info->multi_nm);
	return (SUCCESS);
}