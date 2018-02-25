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

static struct ar_hdr *next_ar(info_obj_t *info, size_t *idx)
{
	struct ar_hdr *ar = info->finfo.vadress + *idx;

	if (*idx == info->finfo.size)
		return (NULL);
	if (out_of_map(info, ar, sizeof(struct ar_hdr))) {
		return (NULL);
	}
	*idx += atol(ar->ar_size) + sizeof(struct ar_hdr);
	if (out_of_map(info, info->finfo.vadress + *idx, 0)) {
		return (NULL);
	}
	return (info->finfo.vadress + *idx);
}

static int create_header(struct ar_hdr *header, info_obj_t *info,
	info_obj_t *binfo, struct ar_hdr *names)
{
	int len = my_strlen(header->ar_name, '/', 16);
	void *save;
	void *end = binfo->finfo.vadress + binfo->finfo.size;

	if (len || names == 0)
		info->finfo.name = strndup(header->ar_name, len);
	else {
		save = (void *) names + sizeof(struct ar_hdr) +
		       atoll(header->ar_name + 1);
		if (out_of_map(info, save, 0))
			return (ERROR);
		len = my_strlen(save, '/', -1);
		info->finfo.name = strndup(save, len);
	}
	info->finfo.size = (size_t) atoll(header->ar_size);
	if (info->finfo.size + (void *) header + sizeof(struct ar_hdr) > end)
		info->finfo.size = (size_t) end - ((size_t) header +
		                                   sizeof(struct ar_hdr));
	info->finfo.vadress = (void *)header + sizeof(struct ar_hdr);
	return (SUCCESS);
}

static int print_subfile(info_obj_t *info, struct ar_hdr *names_h,
	struct ar_hdr *current_h)
{
	info_obj_t new_info;

	memcpy(&new_info, info, sizeof(info_obj_t));
	if (create_header(current_h, &new_info, info, names_h) == ERROR)
		return (my_puterror(info, "Can't create subfile informations"));
	if (check_elf(&new_info) == ERROR)
		return (ERROR);
	print_elf(&new_info);
	free(new_info.finfo.name);
	return (SUCCESS);
}

int check_ar(info_obj_t *info)
{
	if (info->finfo.size < SARMAG)
		return (ERROR);
	if (memcmp(info->finfo.vadress, ARMAG, SARMAG) != 0)
		return (ERROR);
	return (SUCCESS);
}

int print_ar(info_obj_t *info)
{
	struct ar_hdr *name_header = NULL;
	size_t i = SARMAG;
	struct ar_hdr *n = info->finfo.vadress + i;

	printf("In archive %s:\n", info->finfo.name);
	while (!name_header && n &&
	       !out_of_map(info, n, sizeof(struct ar_hdr)) &&
	       my_strlen(n->ar_name, '/', 16) == 0) {
		if (!strncmp(n->ar_name, "//", 2))
			name_header = n;
		n = next_ar(info, &i);
	}
	while (n && !out_of_map(info, n, sizeof(struct ar_hdr))) {
		print_subfile(info, name_header, n);
		n = next_ar(info, &i);
	}
	return (SUCCESS);
}