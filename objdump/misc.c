/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** misc.c
*/
#include <glob.h>
#include <stdio.h>
#include "objdump.h"

int my_perror(info_obj_t *infos)
{
	if (infos)
		fprintf(stderr, "%s: %s: ", infos->name, infos->finfo.name);
	perror("");
	return (ERROR);
}

int my_puterror(info_obj_t *infos, char const *str)
{
	if (infos)
		fprintf(stderr, "%s: %s: ", infos->name, infos->finfo.name);
	fprintf(stderr, "%s\n", str);
	return (ERROR);
}

int my_strlen(void *ptr, char c)
{
	char *str = ptr;
	int i = -1;

	while (str[++i] != c && str[i]);
	return (i);
}
