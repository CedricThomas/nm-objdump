/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** misc.c
*/
#include <stdio.h>
#include <memory.h>
#include <ctype.h>
#include "nm.h"

int nm_perror(info_nm_t *infos)
{
	if (infos)
		fprintf(stderr, "%s: %s: ", infos->name, infos->finfo.name);
	perror("");
	return (1);
}

int nmputerror(info_nm_t *infos, char const *str)
{
	if (infos)
		fprintf(stderr, "%s: %s: ", infos->name, infos->finfo.name);
	fprintf(stderr, "%s\n", str);
	return (1);
}

int is_nm_printable(size_t x)
{
	return x != STT_SECTION && x != STT_FILE;
}

int spec_strcmp(char *a, char *b)
{
	size_t i = 0;
	size_t j = 0;

	while (a[i] && b[j]) {
		while (a[i] == '_')
			i++;
		while (b[j] == '_')
			j++;
		if (tolower(a[i]) != tolower(b[j]))
			break;
		i += 1;
		j += 1;
	}
	return tolower(a[i]) - tolower(b[j]);
}