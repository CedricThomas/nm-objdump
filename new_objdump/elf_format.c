/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** elf_format.c
*/
#include <stdlib.h>
#include <glob.h>
#include <elf.h>
#include "objdump.h"


#ifdef ARCHI32
Elf_Shdr *elf_get_sheader_32(info_obj_t *infos)
#elif ARCHI64
Elf_Shdr *elf_get_sheader_64(info_obj_t *infos)
#endif
{
	void *head = (infos->finfo.vadress +
	              ((Elf_Ehdr *) infos->finfo.vadress)->e_shoff);

	if (out_of_map(infos, head, sizeof(Elf_Shdr)))
		return (NULL);
	return (head);
}

#ifdef ARCHI32
char *get_section_name_32(info_obj_t *infos, Elf_Shdr *shead)
#elif ARCHI64
char *get_section_name_64(info_obj_t *infos, Elf_Shdr *shead)
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
