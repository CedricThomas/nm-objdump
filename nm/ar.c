/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** ar.c
*/
#include <ar.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include "nm.h"

static struct ar_hdr *next_ar(struct ar_hdr *beg, size_t *idx, size_t end)
{
	struct ar_hdr *ar = (void *)beg + *idx;

	*idx += atol(ar->ar_size) + sizeof(struct ar_hdr);
	if (*idx >= end)
		return (NULL);
	return ((void *)beg + *idx);
}

static void create_header(struct ar_hdr *header, info_nm_t *info, size_t sz, struct ar_hdr *names)
{
	int len = my_strlen(header->ar_name, '/');
	void *save;

	if (len)
		info->finfo.name = strndup(header->ar_name, len);
	else {
		save = (void *) names + sizeof(struct ar_hdr) +
		                  atoll(header->ar_name + 1);
		len = my_strlen(save, '/');
		info->finfo.name = strndup(save, len);
	}
	info->finfo.size = (size_t) atoll(header->ar_size);
	info->finfo.vadress = info->finfo.vadress + sz + sizeof(struct ar_hdr);
}

static int print_subfile(info_nm_t *info, struct ar_hdr *names_h,
	struct ar_hdr *current_h, size_t *current_idx)
{
	info_nm_t new_info;

	memcpy(&new_info, info, sizeof(info_nm_t));
	create_header(current_h, &new_info, *current_idx, names_h);
	printf("\n%s:\n", new_info.finfo.name);
	if (check_elf(&new_info))
		return (1);
	if (extract_symbol_list(&new_info))
		return (1);
	print_file(&new_info.finfo, 0);
	free(new_info.finfo.name);
	return (0);
}

int check_ar(info_nm_t *info)
{
	if (memcmp(info->finfo.vadress, ARMAG, SARMAG) != 0)
		return (HEADER_ERROR);
	return (SUCCESS);
}

int print_ar(info_nm_t *info)
{
	struct ar_hdr *name_header;
	struct ar_hdr *n;
	size_t i = SARMAG;

	if (info->multi_nm)
		printf("\n%s:\n", info->finfo.name);
	name_header = next_ar(info->finfo.vadress, &i, info->finfo.size);
	n = next_ar(info->finfo.vadress, &i, info->finfo.size);
	while (n) {
		print_subfile(info, name_header, n, &i);
		n = next_ar(info->finfo.vadress, &i, info->finfo.size);
	}
	munmap(info->finfo.vadress, info->finfo.size);
	return (0);
}