/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** nm.c
*/
#include <stdio.h>
#include <elf.h>
#include <sys/mman.h>
#include <memory.h>
#include <ar.h>
#include <stdlib.h>
#include <zconf.h>
#include "nm.h"

char print_type(Elf64_Sym *sym, Elf64_Shdr *shdr)
{
	char c;

	if (ELF64_ST_BIND(sym->st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF64_ST_BIND(sym->st_info) == STB_WEAK)
	{
		c = 'W';
		if (sym->st_shndx == SHN_UNDEF)
			c = 'w';
	}
	else if (ELF64_ST_BIND(sym->st_info) == STB_WEAK && ELF64_ST_TYPE(sym->st_info) == STT_OBJECT)
	{
		c = 'V';
		if (sym->st_shndx == SHN_UNDEF)
			c = 'v';
	}
	else if (sym->st_shndx == SHN_UNDEF)
		c = 'U';
	else if (sym->st_shndx == SHN_ABS)
		c = 'A';
	else if (sym->st_shndx == SHN_COMMON)
		c = 'C';
	else if (shdr[sym->st_shndx].sh_type == SHT_NOBITS
	         && shdr[sym->st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'B';
	else if (shdr[sym->st_shndx].sh_type == SHT_PROGBITS
	         && shdr[sym->st_shndx].sh_flags == SHF_ALLOC)
		c = 'R';
	else if (shdr[sym->st_shndx].sh_type == SHT_PROGBITS
	         && shdr[sym->st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'D';
	else if (shdr[sym->st_shndx].sh_type == SHT_PROGBITS
	         && shdr[sym->st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		c = 'T';
	else if (shdr[sym->st_shndx].sh_type == SHT_DYNAMIC)
		c = 'D';
	else
		c = '?';
	if (ELF64_ST_BIND(sym->st_info) == STB_LOCAL && c != '?')
		c += 32;
	return c;
}

static void print_syms(info_file_t *info)
{
	void *sheader = elf_get_sheader(info->vadress);

	for (list_t n = info->sym_links; n != NULL; n = n->next) {
		if (((Elf64_Sym *)n->value)->st_value)
			printf("%016lx ", ((Elf64_Sym *)n->value)->st_value);
		else
			printf("%16s ", "");
		printf("%c ", print_type(n->value, sheader));
		printf("%s\n", n->name);
	}
}

static int print_file(info_file_t *info, int multi)
{
	if (multi)
		printf("\n%s:\n", info->name);
	print_syms(info);
	list_clear(&info->sym_links);
	return (0);
}

static int my_strlen(void *ptr, char c)
{
	char *str = ptr;
	int i = -1;

	while (str[++i] != c && str[i]);
	return (i);
}

static struct ar_hdr *next_ar(struct ar_hdr *beg, size_t *idx, size_t end)
{
	struct ar_hdr *ar = (void *)beg + *idx;

	*idx += atol(ar->ar_size) + sizeof(struct ar_hdr);
	if (*idx >= end)
		return (NULL);
	return ((void *)beg + *idx);
}

static int ar_format(info_nm_t *info)
{
	info_nm_t tmp_info;
	struct ar_hdr *name_header;
	size_t i = SARMAG;
	int len = 0;

	if (memcmp(info->finfo.vadress, ARMAG, SARMAG) != 0)//TODO check if ar file
		return (nmputerror(info, "Invalid ar header."));
	memcpy(&tmp_info, info, sizeof(info_nm_t));
	if (info->multi_nm)
		printf("\n%s:\n", info->finfo.name);
	name_header = next_ar(info->finfo.vadress, &i, info->finfo.size);
	for (struct ar_hdr *n = next_ar(info->finfo.vadress, &i,
	                                info->finfo.size); n; ) {
		tmp_info.finfo.name = n->ar_name;
		tmp_info.finfo.size = (size_t) atoll(n->ar_size);
		tmp_info.finfo.vadress = info->finfo.vadress + i +
			sizeof(struct ar_hdr);
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

//TODO munmap nettly

int nm(info_nm_t *info)
{
	int format = 0;

	if (map_file(&info->finfo))
		return (nm_perror(info));
	ar_format(info);
	return (1);
	 format = check_elf(info);
	if (format == SUCCESS) {
		if (extract_symbol_list(info))
			return (1);
		print_file(&info->finfo, info->multi_nm);
	} else if (format == HEADER_ERROR) {
		if (ar_format(info))
			return (1);
	} else
		return (1);
	return (0);
}