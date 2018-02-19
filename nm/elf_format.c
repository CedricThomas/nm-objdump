/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** elf_format.c
*/
#include <glob.h>
#include <zconf.h>
#include <memory.h>
#include <elf.h>
#include "nm.h"

#ifdef ARCHI32
char *get_section_name_32(Elf_Ehdr *ehead, Elf_Shdr *rshead,
Elf_Shdr *shead)
#elif ARCHI64
char *get_section_name_64(Elf_Ehdr *ehead, Elf_Shdr *rshead,
Elf_Shdr *shead)
#endif
{
	Elf_Shdr *sh_strtab = &rshead[ehead->e_shstrndx];
	char *sh_strtab_p = (void *)ehead + sh_strtab->sh_offset;
	return (sh_strtab_p + shead->sh_name);
}

#ifdef ARCHI32
Elf_Shdr *elf_get_sheader_32(Elf_Ehdr *hdr)
#elif ARCHI64
Elf_Shdr *elf_get_sheader_64(Elf_Ehdr *hdr)
#endif
{
	return (Elf_Shdr *) ((void *) hdr + hdr->e_shoff);
}

#ifdef ARCHI32
Elf_Shdr *elf_get_section_32(info_file_t *info, char const *sname)
#elif ARCHI64
Elf_Shdr *elf_get_section_64(info_file_t *info, char const *sname)
#endif
{
	Elf_Ehdr *eheader = info->vadress;
	#ifdef ARCHI32
	Elf_Shdr *sheader = elf_get_sheader_32(info->vadress);
	#elif ARCHI64
	Elf_Shdr *sheader = elf_get_sheader_64(info->vadress);
	#endif
	void *section = NULL;
	size_t sh_size;
	char *cname;

	if ((void *) sheader == (void *) eheader ||
	    (!sheader->sh_size && !eheader->e_shnum) ||
	    eheader->e_shstrndx == SHN_UNDEF)
		return (NULL);
	sh_size = !sheader->sh_size ? eheader->e_shnum : sheader->sh_size;
	for (size_t i = 0; i < sh_size; i++) {
		#ifdef ARCHI32
		cname = get_section_name_32(eheader, sheader, &sheader[i]);
		#elif ARCHI64
		cname = get_section_name_64(eheader, sheader, &sheader[i]);
		#endif
		if (!strcmp(sname, cname)) {
			section = &sheader[i];
			break;
		}
	}
	return section;
}
