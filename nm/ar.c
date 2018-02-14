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

static void create_header(struct ar_hdr *header, info_nm_t *info, size_t sz)
{
	info->finfo.name = header->ar_name;
	info->finfo.size = (size_t) atoll(header->ar_size);
	info->finfo.vadress = info->finfo.vadress + sz + sizeof(struct ar_hdr);
}

int ar_format(info_nm_t *info)
{
	info_nm_t tmp_info;
	struct ar_hdr *name_header;
	size_t i = SARMAG;
	int len = 0;

	memcpy(&tmp_info, info, sizeof(info_nm_t));
	if (info->multi_nm)
		printf("\n%s:\n", info->finfo.name);
	name_header = next_ar(info->finfo.vadress, &i, info->finfo.size);
	for (struct ar_hdr *n = next_ar(info->finfo.vadress, &i,
	                                info->finfo.size); n; ) {

		len = (int) my_strlen(n->ar_name, '/');
		if (len)
			printf("\n%.*s:\n", len, n->ar_name);
		else {
			n = (void *) name_header + sizeof(struct ar_hdr) + atoll(n->ar_name + 1);
			printf("\n%.*s:\n", my_strlen(n, '/'), (char *) n);
		}
		if (!check_elf(&tmp_info)) {
			extract_symbol_list(&tmp_info);
		}
		print_file(&tmp_info.finfo, 0);
		n = next_ar(info->finfo.vadress, &i, info->finfo.size);
	}
	return (0);
}

static void print_subfile(info_nm_t *info, struct ar_hdr *names_h,
	struct ar_hdr *current_h, size_t *current_idx)
{
	info_nm_t new_info;
	int len = my_strlen(current_h->ar_name, '/');

	memcpy(&new_info, info, sizeof(info_nm_t));
	create_header(current_h, &new_info, *current_idx);
	if (len)
		printf("\n%.*s:\n", len, current_h->ar_name);
	else {
		current_h = (void *) names_h + sizeof(struct ar_hdr) + atoll(current_h->ar_name + 1);
		printf("\n%.*s:\n", my_strlen(current_h, '/'), (char *) current_h);
	}
	if (!check_elf(&new_info)) {
		extract_symbol_list(&new_info);
	}
	print_file(&new_info.finfo, 0);
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