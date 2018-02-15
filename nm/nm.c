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

int map_file(info_file_t *finfo)
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

static void print_syms_64(info_file_t *info)
{
	char c;

	for (list_t n = info->sym_links; n != NULL; n = n->next) {
		c = print_type_64(n->value, info);
		if (((Elf64_Sym *)n->value)->st_value ||
			(c != 'U' && c != 'w'))
			printf("%016lx ", ((Elf64_Sym *)n->value)->st_value);
		else
			printf("%16s ", "");
		printf("%c ", c);
		printf("%s\n", n->name);
	}
}

static void print_syms_32(info_file_t *info)
{
	char c;

	for (list_t n = info->sym_links; n != NULL; n = n->next) {
		c = print_type_32(n->value, info);
		if (((Elf32_Sym *)n->value)->st_value ||
			(c != 'U' && c != 'w'))
			printf("%08x ", ((Elf32_Sym *)n->value)->st_value);
		else
			printf("%8s ", "");
		printf("%c ", c);
		printf("%s\n", n->name);
	}
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
			return (1);
	} else if (check_ar(info) == SUCCESS) {
		if (print_ar(info))
			return (1);
	} else
		return (nmputerror(info, "Invalid elf header."));
	return (0);
}