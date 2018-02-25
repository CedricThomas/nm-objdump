/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** main.c
*/

#include <memory.h>
#include "objdump.h"

static int multi_obj(int argc, char **argv, info_obj_t *info)
{
	int ret = 0;
	int last;

	for (int i = 1; i < argc; i++) {
		info->finfo.name = argv[i];
		last = objdump(info);
		if (last)
			ret = last;
		memset(&info->finfo, 0, sizeof(info_file_t));
	}
	return (ret);
}

int main(int argc, char **argv)
{
	info_obj_t info;

	memset(&info, 0, sizeof(info_obj_t));
	info.name = argv[0];
	if (argc == 1) {
		info.finfo.name = "a.out";
		return (objdump(&info));
	}
	if (argc == 2) {
		info.finfo.name = argv[1];
		return (objdump(&info));
	} else
		return (multi_obj(argc, argv, &info));
}