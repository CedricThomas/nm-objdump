/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** elf_32.c
*/
#include <glob.h>
#include <zconf.h>
#include <memory.h>
#include "nm.h"

char *get_section_name_32(Elf32_Ehdr *ehead,
	Elf32_Shdr *rshead, Elf32_Shdr *shead)
{
	Elf32_Shdr *sh_strtab = &rshead[ehead->e_shstrndx];
	char *sh_strtab_p = (void *)ehead + sh_strtab->sh_offset;
	return (sh_strtab_p + shead->sh_name);
}

Elf32_Shdr *elf_get_sheader_32(Elf32_Ehdr *hdr)
{
	return (Elf32_Shdr *) ((size_t) hdr + hdr->e_shoff);
}

Elf32_Shdr *elf_get_section_32(info_file_t *info, char const *sname)
{
	Elf32_Ehdr *eheader = info->vadress;
	Elf32_Shdr *sheader = elf_get_sheader_32(info->vadress);
	void *section = NULL;
	size_t sh_size;
	char *cname;

	if ((void *) sheader == (void *) eheader ||
	    (!sheader->sh_size && !eheader->e_shnum) ||
	    eheader->e_shstrndx == SHN_UNDEF)
		return (NULL);
	sh_size = !sheader->sh_size ? eheader->e_shnum : sheader->sh_size;
	for (size_t i = 0; i < sh_size; i++) {
		cname = get_section_name_32(eheader, sheader, &sheader[i]);
		if (!strcmp(sname, cname)) {
			section = &sheader[i];
			break;
		}
	}
	return section;
}
