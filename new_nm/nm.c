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

static int map_file(info_nm_t *i)
{
	info_file_t *finfo = &i->finfo;
	struct stat s;
	int fd;

	if (access(finfo->name, F_OK) == -1)
		return (nmsputerror(i, "No such file"));
	fd = open(finfo->name, O_RDONLY);
	if (fd == -1)
		return (nmputerror(i, "Permission denied"));
	if (fstat(fd, &s) == -1)
		return (nmputerror(i, "Permission denied"));
	finfo->size = (size_t) s.st_size;
	finfo->vadress = mmap(0, finfo->size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (finfo->vadress == MAP_FAILED)
		return (nmputerror(i, "File format not recognized"));
	close(fd);
	return (SUCCESS);
}

static int extract_type(info_nm_t *info)
{
	for (list_t n = info->finfo.sym_links; n != NULL; n = n->next) {
		if (info->finfo.archi == ELFCLASS64) {
			if (print_type_64(info, n) == ERROR)
				return (ERROR);
		} else {
			if (print_type_32(info, n) == ERROR)
				return (ERROR);
		}
	}
	return (SUCCESS);
}

int extract_symbol_list(info_nm_t *infos)
{
	if (infos->finfo.archi == ELFCLASS64) {
		if (create_symbol_list_64(infos) == ERROR)
			return (ERROR);
	} else {
		if (create_symbol_list_32(infos) == ERROR)
			return (ERROR);
	}
	list_sort(infos->finfo.sym_links);
	if (extract_type(infos) == ERROR)
		return (ERROR);
	return (SUCCESS);
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
	return (SUCCESS);
}

int nm(info_nm_t *info)
{
	int ret = ERROR;

	if (map_file(info) == ERROR)
		return (ERROR);
	if (check_ar(info) == SUCCESS) {
		ret = SUCCESS;
		if (print_ar(info))
			return (my_munmap(info, ERROR));
	} else if (check_elf(info) == SUCCESS) {
		ret = SUCCESS;
		if (print_elf(info))
			return (my_munmap(info, ERROR));
	}
	return (my_munmap(info, ret));
}