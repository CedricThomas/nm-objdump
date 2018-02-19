/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** elf_format.c
*/
#include <glob.h>
#include <elf.h>

char *get_section_name_64(Elf64_Ehdr *ehead,
	Elf64_Shdr *rshead, Elf64_Shdr *shead)
{
	char *sh_strtab_p = (void *)ehead +
		rshead[ehead->e_shstrndx].sh_offset;
	return (sh_strtab_p + shead->sh_name);
}

Elf64_Shdr *elf_get_sheader_64(Elf64_Ehdr *hdr)
{
	return ((void *) hdr + hdr->e_shoff);
}

char *get_section_name_32(Elf32_Ehdr *ehead, Elf32_Shdr *rshead,
Elf32_Shdr *shead)
{
	char *sh_strtab_p = (void *)ehead +
	                    rshead[ehead->e_shstrndx].sh_offset;
	return (sh_strtab_p + shead->sh_name);
}

Elf32_Shdr *elf_get_sheader_32(Elf32_Ehdr *hdr)
{
	return ((void *) hdr + hdr->e_shoff);
}