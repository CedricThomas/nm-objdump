/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** print.c
*/
#include <stdio.h>
#include "flags.h"
#include "objdump.h"

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

int print_sections(info_file_t *info)
{
	if (info->archi == ELFCLASS64)
		print_sections_64(info);
	else
		print_sections_32(info);
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
