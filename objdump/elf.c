/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** elf.c
*/
#include <glob.h>
#include <memory.h>
#include "objdump.h"

int print_elf(info_file_t *info)
{
	print_header(info);
	print_sections(info);
	return (0);
}

int check_elf(info_obj_t *info)
{
	Elf64_Ehdr *elf = info->finfo.vadress;
	char header[] = {
		ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3
	};

	if (memcmp(info->finfo.vadress, header, sizeof(header)) != 0)
		return (HEADER_ERROR);
	if (elf->e_ident[EI_CLASS] == ELFCLASSNONE)
		return (my_puterror(info, "File architecture not supported."));
	info->finfo.archi = elf->e_ident[EI_CLASS];
	if(elf->e_ident[EI_DATA] != ELFDATA2LSB)
		return (my_puterror(info, "Unsupported ELF File byte order."));
	if (elf->e_ident[EI_VERSION] != EV_CURRENT)
		return (my_puterror(info, "Unsupported ELF File version."));
	return (0);
}
