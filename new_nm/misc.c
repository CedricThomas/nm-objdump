/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** misc.c
*/
#include <stdio.h>
#include <sys/mman.h>
#include "nm.h"

int out_of_map(info_nm_t *info, void *addr, size_t len)
{
	return (!(info->finfo.vadress <= addr &&
		addr + len <= info->finfo.vadress + info->finfo.size));
}

int my_munmap(info_nm_t *info, int ret)
{
	if (info->finfo.vadress)
		munmap(info->finfo.vadress, info->finfo.size);
	return (ret);
}

int nmputerror(info_nm_t *infos, char const *str)
{
	if (infos)
		fprintf(stderr, "%s: %s: ", infos->name, infos->finfo.name);
	fprintf(stderr, "%s\n", str);
	return (ERROR);
}

int nmsputerror(info_nm_t *infos, char const *str)
{
	if (infos)
		fprintf(stderr, "%s: '%s': ", infos->name, infos->finfo.name);
	fprintf(stderr, "%s\n", str);
	return (ERROR);
}

int is_nm_printable(size_t x)
{
	return x != STT_SECTION && x != STT_FILE;
}