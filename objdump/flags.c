/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** flags.c
*/
#include <glob.h>
#include <elf.h>
#include <stdio.h>
#include "flags.h"

static const size_t flags_values[] = {
	HAS_RELOC,
	EXEC_P,
	HAS_SYMS,
	DYNAMIC,
	D_PAGED
};

static const char *flags_names[] = {
	"HAS_RELOC",
	"EXEC_P",
	"HAS_SYMS",
	"DYNAMIC",
	"D_PAGED"
};

void print_flags(size_t flags)
{
	int coma = 0;

	for (size_t i = 0; i < 5; i++) {
		if ((flags & flags_values[i]) == flags_values[i] && coma)
			printf(", ");
		if ((flags & flags_values[i]) == flags_values[i]) {
			printf("%s", flags_names[i]);
			coma = 1;
		}
	}
	printf("\n");
}

size_t catch_flags_64(Elf64_Shdr *shdr, Elf64_Ehdr *ehdr,
	size_t size_headers)
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

size_t catch_flags_32(Elf32_Shdr *shdr, Elf32_Ehdr *ehdr,
	size_t size_headers)
{
	size_t flags = 0;

	for (size_t i = 0; i < size_headers; i ++)
		if (shdr[i].sh_type & (SHT_RELA | SHT_REL)) {
			flags += HAS_RELOC;
			break;
		}
	flags += HAS_SYMS;
	if (ehdr->e_type == EXEC_P) {
		flags -= HAS_RELOC;
		flags += EXEC_P | D_PAGED;
	}
	if (ehdr->e_type == DYNAMIC || ehdr->e_type == ET_DYN) {
		flags -= HAS_RELOC;
		flags += DYNAMIC | D_PAGED;
	}
	return (flags);
}
