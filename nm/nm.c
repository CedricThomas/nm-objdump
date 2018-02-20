/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** nm.c
*/
#include <stdio.h>
#include <elf.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <zconf.h>
#include "nm.h"

static int map_file(info_file_t *finfo)
{
	struct stat s;
	int fd;

	fd = open(finfo->name, O_RDONLY);
	if (fd == -1)
		return (1);
	if (fstat(fd, &s) == -1)
		return (1);
	finfo->size = (size_t) s.st_size;
	finfo->vadress = mmap(0, finfo->size, PROT_READ,
	                      MAP_PRIVATE, fd, 0);
	if (finfo->vadress == MAP_FAILED)
		return (1);
	close(fd);
	return (0);
}

int extract_symbol_list(info_nm_t *infos)
{
	if (infos->finfo.archi == ELFCLASS64) {
		if (create_symbol_list_64(infos))
			return (1);
	} else {
		if (create_symbol_list_32(infos))
			return (1);
	}
	list_sort(infos->finfo.sym_links);
	return (0);
}

int print_file(info_file_t *info, int multi)
{
	if (multi)
		printf("\n%s:\n", info->name);
	if (info->archi == ELFCLASS32)
		print_syms_32(info);
	else
		print_syms_64(info);
	list_clear(&info->sym_links);
	return (0);
}

int nm(info_nm_t *info)
{
	int format = 0;

	if (map_file(&info->finfo))
		return (nm_perror(info));
	format = check_elf(info);
	if (format == SUCCESS) {
		if (print_elf(info))
			return (ERROR);
	} else if (check_ar(info) == SUCCESS) {
		if (print_ar(info))
			return (ERROR);
	} else
		return (nmputerror(info, "Invalid elf header."));
	return (0);
}