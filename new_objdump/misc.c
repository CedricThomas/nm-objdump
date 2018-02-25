/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** misc.c
*/
#include <glob.h>
#include <sys/mman.h>
#include <stdio.h>
#include "objdump.h"


int out_of_map(info_obj_t *info, void *addr, size_t len)
{
	return (!(info->finfo.vadress <= addr &&
	          addr + len <= info->finfo.vadress + info->finfo.size));
}

int my_munmap(info_obj_t *info, int ret)
{
	if (info->finfo.vadress)
		munmap(info->finfo.vadress, info->finfo.size);
	return (ret);
}

int my_specputerror(info_obj_t *infos, char const *str)
{
	if (infos)
		fprintf(stderr, "%s: '%s': ", infos->name, infos->finfo.name);
	fprintf(stderr, "%s\n", str);
	return (ERROR);
}

int my_puterror(info_obj_t *infos, char const *str)
{
	if (infos)
		fprintf(stderr, "%s: %s: ", infos->name, infos->finfo.name);
	fprintf(stderr, "%s\n", str);
	return (ERROR);
}

int my_strlen(void *ptr, char c, int max)
{
	char *str = ptr;
	int i = -1;

	while ((i < max || max == -1) && str[++i] != c && str[i]);
	return (i);
}