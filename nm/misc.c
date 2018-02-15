/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** misc.c
*/
#include <stdio.h>
#include "nm.h"

int nm_perror(info_nm_t *infos)
{
	if (infos)
		fprintf(stderr, "%s: %s: ", infos->name, infos->finfo.name);
	perror("");
	return (ERROR);
}

int nmputerror(info_nm_t *infos, char const *str)
{
	if (infos)
		fprintf(stderr, "%s: %s: ", infos->name, infos->finfo.name);
	fprintf(stderr, "%s\n", str);
	return (ERROR);
}

int is_nm_printable(size_t x)
{
	return x != STT_SECTION && x != STT_FILE;
}