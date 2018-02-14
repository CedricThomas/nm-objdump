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

char *get_section_name(Elf64_Ehdr *ehead,
	Elf64_Shdr *rshead, Elf64_Shdr *shead)
{
	Elf64_Shdr *sh_strtab = &rshead[ehead->e_shstrndx];
	char *sh_strtab_p = (void *)ehead + sh_strtab->sh_offset;
	return (sh_strtab_p + shead->sh_name);
}

Elf64_Shdr *elf_get_sheader(Elf64_Ehdr *hdr)
{
	return (Elf64_Shdr *) ((size_t) hdr + hdr->e_shoff);
}

Elf64_Shdr *elf_get_section(info_nm_t *info, char const *sname)
{
	Elf64_Ehdr *eheader = info->finfo.vadress;
	Elf64_Shdr *sheader = elf_get_sheader(info->finfo.vadress);
	void *section = NULL;
	size_t sh_size;
	char *cname;

	if ((void *) sheader == (void *) eheader ||
		(!sheader->sh_size && !eheader->e_shnum) ||
		eheader->e_shstrndx == SHN_UNDEF)
		return (NULL);
	sh_size = !sheader->sh_size ? eheader->e_shnum : sheader->sh_size;
	for (size_t i = 0; i < sh_size; i++) {
		cname = get_section_name(eheader, sheader, &sheader[i]);
		if (!strcmp(sname, cname)) {
			section = &sheader[i];
			break;
		}
	}
	return section;
}

int check_elf(info_nm_t *info)
{
	Elf64_Ehdr *elf = info->finfo.vadress;
	char header[] = {
		ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3
	};

	if (memcmp(info->finfo.vadress, header, sizeof(header)) != 0)
		return (HEADER_ERROR);
	if (elf->e_type == ELFCLASSNONE)
		return (nmputerror(info, "File architecture not supported."));
	info->finfo.archi = elf->e_type;
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