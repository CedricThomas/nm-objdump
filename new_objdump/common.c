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
int print_sections_64(info_obj_t *info)
#elif ARCHI32
int print_sections_32(info_obj_t *info)
#endif
{
	#ifdef ARCHI64
	Elf_Shdr *shder = elf_get_sheader_64(info);
	#elif ARCHI32
	Elf_Shdr *shder = elf_get_sheader_32(info);
	#endif
	Elf_Ehdr *ehdr = info->finfo.vadress;

	char *name;

	for (size_t i = 1; i < ehdr->e_shnum; ++i) {
		if (out_of_map(info, shder + i, sizeof(Elf_Shdr)))
			return (my_puterror(info, "File truncated"));
		#ifdef ARCHI64
		name = get_section_name_64(info, &shder[i]);
		#elif ARCHI32
		name = get_section_name_32(info, &shder[i]);
		#endif
		if (!name || shder[i].sh_size == 0 ||
			shder[i].sh_type == SHT_NULL)
			continue;
		if (out_of_map(info, info->finfo.vadress + shder[i].sh_offset,
			shder[i].sh_size))
			return (my_puterror(info, "File truncated"));
		printf("Contents of section %s:\n", name);
		print_hexa(info->finfo.vadress + shder[i].sh_offset,
		           shder[i].sh_addr, shder[i].sh_size);
	}
	return (SUCCESS);
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
			flags |= HAS_RELOC;
			break;
		}
	flags |= HAS_SYMS;
	if (ehdr->e_type == ET_EXEC) {
		flags &= ~HAS_RELOC;
		flags |= EXEC_P | D_PAGED;
	}
	if (ehdr->e_type == ET_DYN) {
		flags &= ~HAS_RELOC;
		flags |= DYNAMIC | D_PAGED;
	}
	return (flags);
}

#ifdef ARCHI64
int print_header_64(info_obj_t *info)
#elif ARCHI32
int print_header_32(info_obj_t *info)
#endif
{
	Elf_Ehdr *ehdr = info->finfo.vadress;
	size_t flags;

	#ifdef ARCHI64
	if (out_of_map(info, elf_get_sheader_64(info),
		ehdr->e_shnum * sizeof(Elf_Shdr)))
		return (my_puterror(info, "File truncated"));
	flags = catch_flags_64(elf_get_sheader_64(info), ehdr, ehdr->e_shnum);
	printf("\n");
	printf("%s:     file format %s\n", info->finfo.name, "elf64-x86-64");
	printf("architecture: %s, flags 0x%08lx:\n", "i386:x86-64", flags);
	print_flags(flags);
	printf("start address 0x%016lx\n", ehdr->e_entry);
	#elif ARCHI32
	if (out_of_map(info, elf_get_sheader_32(info),
		ehdr->e_shnum * sizeof(Elf_Shdr)))
		return (my_puterror(info, "File truncated"));
	flags = catch_flags_32(elf_get_sheader_32(info), ehdr, ehdr->e_shnum);
	printf("\n");
	printf("%s:     file format %s\n", info->finfo.name, "elf32-i386");
	printf("architecture: %s, flags 0x%08lx:\n", "i386", flags);
	print_flags(flags);
	printf("start address 0x%08x\n", ehdr->e_entry);
	#endif
	printf("\n");
	return (SUCCESS);
}