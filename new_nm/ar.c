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

static struct ar_hdr *next_ar(info_nm_t *info, size_t *idx)
{
	struct ar_hdr *ar = info->finfo.vadress + *idx;

	if (*idx == info->finfo.size)
		return (NULL);
	if (out_of_map(info, ar, sizeof(struct ar_hdr))) {
		nmputerror(info, "File truncated");
		return (NULL);
	}
	*idx += atol(ar->ar_size) + sizeof(struct ar_hdr);
	if (out_of_map(info, info->finfo.vadress + *idx, 0)) {
		nmputerror(info, "File truncated");
		return (NULL);
	}
	return (info->finfo.vadress + *idx);
}

static int create_header(struct ar_hdr *header,
	info_nm_t *info, struct ar_hdr *names)
{
	int len = my_strlen(header->ar_name, '/', 16);
	void *save;

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
	info->finfo.vadress = (void *)header + sizeof(struct ar_hdr);
	return (SUCCESS);
}

static int print_subfile(info_nm_t *info, struct ar_hdr *names_h,
	struct ar_hdr *current_h)
{
	info_nm_t new_info;

	memcpy(&new_info, info, sizeof(info_nm_t));
	if (create_header(current_h, &new_info, names_h) == ERROR)
		return (nmputerror(info, "Can't create subfile informations"));
	if (check_elf(&new_info) == ERROR)
		return (ERROR);
	printf("\n%s:\n", new_info.finfo.name);
	if (extract_symbol_list(&new_info) == ERROR)
		return (ERROR);
	print_file(&new_info.finfo, 0);
	free(new_info.finfo.name);
	return (SUCCESS);
}

int check_ar(info_nm_t *info)
{
	if (info->finfo.size < SARMAG)
		return (ERROR);
	if (memcmp(info->finfo.vadress, ARMAG, SARMAG) != 0)
		return (ERROR);
	return (SUCCESS);
}

int print_ar(info_nm_t *info)
{
	struct ar_hdr *name_header = NULL;
	size_t i = SARMAG;
	struct ar_hdr *n = info->finfo.vadress + i;

	if (info->multi_nm)
		printf("\n%s:\n", info->finfo.name);
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