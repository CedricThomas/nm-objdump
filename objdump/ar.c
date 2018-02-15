/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** ar.c
*/
#include <glob.h>
#include <memory.h>
#include <ar.h>
#include <stdlib.h>
#include <stdio.h>
#include "objdump.h"

static struct ar_hdr *next_ar(struct ar_hdr *beg, size_t *idx, size_t end)
{
	struct ar_hdr *ar = (void *)beg + *idx;

	*idx += atol(ar->ar_size) + sizeof(struct ar_hdr);
	if (*idx >= end)
		return (NULL);
	return ((void *)beg + *idx);
}

static void create_header(struct ar_hdr *header, info_obj_t *info, size_t sz, struct ar_hdr *names)
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

static int print_subfile(info_obj_t *info, struct ar_hdr *names_h,
	struct ar_hdr *current_h, size_t *current_idx)
{
	info_obj_t new_info;

	memcpy(&new_info, info, sizeof(info_obj_t));
	create_header(current_h, &new_info, *current_idx, names_h);
	if (check_elf(&new_info))
		return (1);
	print_elf(&new_info.finfo);
	free(new_info.finfo.name);
	return (0);
}

int print_ar(info_obj_t *info)
{
	struct ar_hdr *name_header;
	struct ar_hdr *n;
	size_t i = SARMAG;

	name_header = next_ar(info->finfo.vadress, &i, info->finfo.size);
	n = next_ar(info->finfo.vadress, &i, info->finfo.size);
	while (n) {
		print_subfile(info, name_header, n, &i);
		n = next_ar(info->finfo.vadress, &i, info->finfo.size);
	}
	return (0);
}

int check_ar(info_obj_t *info)
{
	if (memcmp(info->finfo.vadress, ARMAG, SARMAG) != 0)
		return (HEADER_ERROR);
	return (SUCCESS);
}
