/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** main.c
*/
#include <stdio.h>
#include <memory.h>
#include "nm.h"

static int multi_nm(int argc, char **argv, info_nm_t * infos)
{
	int ret = 0;
	int last;

	infos->multi_nm = 1;
	for (int i = 1; i < argc; i++) {
		infos->finfo.name = argv[i];
		last = nm(infos);
		if (last)
			ret = last;
		memset(&infos->finfo, 0, sizeof(info_file_t));
	}
	return (ret);
}

int main(int argc, char **argv)
{
	info_nm_t infos;

	memset(&infos, 0, sizeof(info_nm_t));
	infos.name = argv[0];
	if (argc == 1) {
		infos.finfo.name = "a.out";
		return (nm(&infos));
	}
	if (argc == 2) {
		infos.finfo.name = argv[1];
		return (nm(&infos));
	} else
		return (multi_nm(argc, argv, &infos));
}