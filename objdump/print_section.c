/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** print_section.c
*/
#include <glob.h>
#include <elf.h>
#include <stdio.h>
#include "objdump.h"

static int print_sections_32(info_file_t *info)
{
	Elf32_Shdr *shder = elf_get_sheader_32(info->vadress);
	Elf32_Ehdr *ehdr = info->vadress;
	char *name;

	for (size_t i = 1; i < ehdr->e_shnum; ++i) {
		name = get_section_name_32(ehdr, shder, &shder[i]);
		if (shder[i].sh_size == 0 || shder[i].sh_type == SHT_NULL)
			continue;
		printf("Contenu de la section %s%c%c:\n", name, 0xc2, 0xa0);
		print_hexa(info->vadress + shder[i].sh_offset,
		shder[i].sh_addr, shder[i].sh_size);
	}
	return (0);
}

static int print_sections_64(info_file_t *info)
{
	Elf64_Shdr *shder = elf_get_sheader_64(info->vadress);
	Elf64_Ehdr *ehdr = info->vadress;
	char *name;

	for (size_t i = 1; i < ehdr->e_shnum; ++i) {
		name = get_section_name_64(ehdr, shder, &shder[i]);
		if (shder[i].sh_size == 0 || shder[i].sh_type == SHT_NULL)
			continue;
		printf("Contenu de la section %s :\n", name);
		print_hexa(info->vadress + shder[i].sh_offset,
		shder[i].sh_addr, shder[i].sh_size);
	}
	return (0);
}

int print_sections(info_file_t *info)
{
	if (info->archi == ELFCLASS64)
		print_sections_64(info);
	else
		print_sections_32(info);
	return (0);
}