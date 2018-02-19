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

int print_header_32(info_file_t *info)
{
	Elf32_Ehdr *ehdr = info->vadress;
	size_t flags;

	flags = catch_flags_32(elf_get_sheader_32(ehdr), ehdr, ehdr->e_shnum);
	printf("\n");
	printf("%s:     format de fichier %s\n", info->name, "elf32-i386");
	printf("architecture: %s, fanions 0x%08lx:\n", "i386", flags);
	print_flags(flags);
	printf("adresse de départ 0x%08x\n", ehdr->e_entry);
	printf("\n");
	return (0);
}

int print_header_64(info_file_t *info)
{
	Elf64_Ehdr *ehdr = info->vadress;
	size_t flags;

	flags = catch_flags_64(elf_get_sheader_64(ehdr), ehdr, ehdr->e_shnum);
	printf("\n");
	printf("%s:     format de fichier %s\n", info->name, "elf64-x86-64");
	printf("architecture: %s, fanions 0x%08lx:\n", "i386:x86-64", flags);
	print_flags(flags);
	printf("adresse de départ 0x%016lx\n", ehdr->e_entry);
	printf("\n");
	return (0);
}

int print_header(info_file_t *info)
{
	if (info->archi == ELFCLASS64)
		print_header_64(info);
	else
		print_header_32(info);
	return (0);
}
