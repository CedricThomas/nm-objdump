/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** print_header.c
*/
#include <stdio.h>
#include <elf.h>
#include "flags.h"
#include "objdump.h"

#ifdef ARCHI64
int print_sections_64(info_file_t *info)
#elif ARCHI32
int print_sections_32(info_file_t *info)
#endif
{
	#ifdef ARCHI64
	Elf_Shdr *shder = elf_get_sheader_64(info->vadress);
	Elf_Ehdr *ehdr = info->vadress;
	#elif ARCHI32
	Elf_Shdr *shder = elf_get_sheader_32(info->vadress);
	Elf_Ehdr *ehdr = info->vadress;
	#endif
	char *name;

	for (size_t i = 1; i < ehdr->e_shnum; ++i) {
		#ifdef ARCHI64
		name = get_section_name_64(ehdr, shder, &shder[i]);
		#elif ARCHI32
		name = get_section_name_32(ehdr, shder, &shder[i]);
		#endif
		if (shder[i].sh_size == 0 || shder[i].sh_type == SHT_NULL)
			continue;
		printf("Contenu de la section %s :\n", name);
		print_hexa(info->vadress + shder[i].sh_offset,
		           shder[i].sh_addr, shder[i].sh_size);
	}
	return (0);
}

#ifdef ARCHI64
size_t catch_flags_64(Elf_Shdr *shdr, Elf_Ehdr *ehdr,
	size_t size_headers)
#elif ARCHI32
size_t catch_flags_32(Elf_Shdr *shdr, Elf_Ehdr *ehdr,
	size_t size_headers)
#endif
{
	size_t flags = 0;

	for (size_t i = 0; i < size_headers; i ++)
		if (shdr[i].sh_type & (SHT_RELA | SHT_REL)) {
			flags += HAS_RELOC;
			break;
		}
	flags += HAS_SYMS;
	if (ehdr->e_type == ET_EXEC) {
		flags -= HAS_RELOC;
		flags += EXEC_P | D_PAGED;
	}
	if (ehdr->e_type == ET_DYN) {
		flags -= HAS_RELOC;
		flags += DYNAMIC | D_PAGED;
	}
	return (flags);
}


#ifdef ARCHI64
int print_header_64(info_file_t *info)
#elif ARCHI32
int print_header_32(info_file_t *info)
#endif
{
	Elf_Ehdr *ehdr = info->vadress;
	size_t flags;

	#ifdef ARCHI64
	flags = catch_flags_64(elf_get_sheader_64(ehdr), ehdr, ehdr->e_shnum);
	printf("\n");
	printf("%s:     format de fichier %s\n", info->name, "elf64-x86-64");
	printf("architecture: %s, fanions 0x%08lx:\n", "i386:x86-64", flags);
	print_flags(flags);
	printf("adresse de départ 0x%016lx\n", ehdr->e_entry);
	#elif ARCHI32
	flags = catch_flags_32(elf_get_sheader_32(ehdr), ehdr, ehdr->e_shnum);
	printf("\n");
	printf("%s:     format de fichier %s\n", info->name, "elf32-i386");
	printf("architecture: %s, fanions 0x%08lx:\n", "i386", flags);
	print_flags(flags);
	printf("adresse de départ 0x%08x\n", ehdr->e_entry);
	#endif
	printf("\n");
	return (0);
}
