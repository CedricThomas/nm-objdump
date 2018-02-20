/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** elf_format.c
*/
#include <glob.h>
#include <elf.h>
#include "objdump.h"


#ifdef ARCHI64
char *get_section_name_64(Elf_Ehdr *ehead,
	Elf_Shdr *rshead, Elf_Shdr *shead)
#elif ARCHI32
char *get_section_name_32(Elf_Ehdr *ehead, Elf_Shdr *rshead, Elf_Shdr *shead)
#endif
{
	char *sh_strtab_p = (void *)ehead +
		rshead[ehead->e_shstrndx].sh_offset;
	return (sh_strtab_p + shead->sh_name);
}

#ifdef ARCHI64
Elf_Shdr *elf_get_sheader_64(Elf_Ehdr *hdr)
#elif ARCHI32
Elf_Shdr *elf_get_sheader_32(Elf_Ehdr *hdr)
#endif
{
	return ((void *) hdr + hdr->e_shoff);
}
