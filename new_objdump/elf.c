/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** elf.c
*/
#include <glob.h>
#include <memory.h>
#include "objdump.h"

int print_elf(info_obj_t *info)
{
	if (print_header(info) == SUCCESS) {
		if (print_sections(info) == ERROR)
			return (ERROR);
	} else
		return (ERROR);
	return (SUCCESS);
}

static inline size_t eheader_size(size_t archi)
{
	if (archi == ELFCLASS32)
		return (sizeof(Elf32_Ehdr));
	else
		return (sizeof(Elf64_Ehdr));
}

static int check_magic_nbr(info_obj_t *info, Elf64_Ehdr *elf)
{
	char header[] = {
		ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3
	};

	if (info->finfo.size < sizeof(header))
		return (my_puterror(info, "File truncated"));
	else if (memcmp(info->finfo.vadress, header, sizeof(header)) != 0)
		return (my_puterror(info, "File format not recognized"));
	else if (info->finfo.size < sizeof(elf->e_ident))
		return (my_puterror(info, "File format not recognized"));
	return (SUCCESS);
}

static int check_basic_infos(info_obj_t *info, Elf64_Ehdr *elf)
{
	if (elf->e_ident[EI_CLASS] == ELFCLASSNONE)
		return (my_puterror(info, "File architecture not supported"));
	info->finfo.archi = elf->e_ident[EI_CLASS];
	if(elf->e_ident[EI_DATA] != ELFDATA2LSB)
		return (my_puterror(info, "File byte order not supported"));
	else if (elf->e_ident[EI_VERSION] != EV_CURRENT)
		return (my_puterror(info, "File ELF version not supported"));
	return (SUCCESS);
}

int check_elf(info_obj_t *info)
{
	Elf64_Ehdr *elf = info->finfo.vadress;

	if (check_magic_nbr(info, elf) == ERROR ||
	    check_basic_infos(info, elf) == ERROR)
		return (ERROR);
	else if (info->finfo.size < eheader_size(info->finfo.archi))
		return (my_puterror(info, "File format not recognized"));
	else {
		if (info->finfo.archi == ELFCLASS32 &&
		    info->finfo.size < ((Elf32_Ehdr *) elf)->e_shoff)
			return (my_puterror(info, "File truncated"));
		else if (info->finfo.archi == ELFCLASS64 &&
		         info->finfo.size < elf->e_shoff)
			return (my_puterror(info, "File truncated"));
	}
	return (SUCCESS);
}
