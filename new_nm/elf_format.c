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
Elf_Shdr *elf_get_sheader_32(info_nm_t *infos)
#elif ARCHI64
Elf_Shdr *elf_get_sheader_64(info_nm_t *infos)
#endif
{
	void *head = (infos->finfo.vadress +
		((Elf_Ehdr *) infos->finfo.vadress)->e_shoff);

	if (out_of_map(infos, head, sizeof(Elf_Shdr)))
		return (NULL);
	return (head);
}

#ifdef ARCHI32
char *get_section_name_32(info_nm_t *infos, Elf_Shdr *shead)
#elif ARCHI64
char *get_section_name_64(info_nm_t *infos, Elf_Shdr *shead)
#endif
{
	#ifdef ARCHI32
	Elf_Shdr *rshead = elf_get_sheader_32(infos);
	#elif ARCHI64
	Elf_Shdr *rshead = elf_get_sheader_64(infos);
	#endif

	if (out_of_map(infos, shead, sizeof(Elf_Shdr)) || rshead == NULL)
		return (NULL);
	Elf_Shdr *sh_strtab = rshead +
		((Elf_Ehdr *)infos->finfo.vadress)->e_shstrndx;
	if (out_of_map(infos, sh_strtab, sizeof(Elf_Shdr)))
		return (NULL);
	char *sh_strtab_p = infos->finfo.vadress + sh_strtab->sh_offset;
	if (out_of_map(infos, sh_strtab_p, sh_strtab->sh_size) &&
		sh_strtab->sh_size > shead->sh_name)
		return (NULL);
	return (sh_strtab_p + shead->sh_name);
}

#ifdef ARCHI32
Elf_Shdr *elf_get_section_32(info_nm_t *info, char const *sname)
#elif ARCHI64
Elf_Shdr *elf_get_section_64(info_nm_t *info, char const *sname)
#endif
{
	Elf_Ehdr *eheader = info->finfo.vadress;
	#ifdef ARCHI32
	Elf_Shdr *sheader = elf_get_sheader_32(info);
	#elif ARCHI64
	Elf_Shdr *sheader = elf_get_sheader_64(info);
	#endif
	void *section = NULL;
	size_t sh_size;
	char *cname;

	if (sheader == NULL || (void *) sheader == (void *) eheader ||
	    (!sheader->sh_size && !eheader->e_shnum) ||
	    eheader->e_shstrndx == SHN_UNDEF)
		return (NULL);
	sh_size = !sheader->sh_size ? eheader->e_shnum : sheader->sh_size;
	for (size_t i = 0; i < sh_size; i++) {
		#ifdef ARCHI32
		cname = get_section_name_32(info, sheader + i);
		#elif ARCHI64
		cname = get_section_name_64(info, sheader + i);
		#endif
		if (cname && !strcmp(sname, cname)) {
			section = sheader + i;
			break;
		}
	}
	return (section);
}
