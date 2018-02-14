/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** nm.h
*/

#ifndef NMOBJDUMP_NM_H
	#define NMOBJDUMP_NM_H

	#include <elf.h>

	#define ERROR 84
	#define HEADER_ERROR 42
	#define SUCCESS 0

typedef struct node_s {
	char *name;
	void *value;
	struct node_s *next;
} node_t;

typedef node_t *list_t;

typedef struct info_file_s {
	const char *name;
	void *vadress;
	size_t size;
	size_t archi;
	list_t sym_links;
} info_file_t;

typedef struct info_nm_s {
	info_file_t finfo;
	char *name;
	int multi_nm;
} info_nm_t;

/*
**nm.c
*/
int nm(info_nm_t *infos);
int print_file(info_file_t *info, int multi);

/*
**elf.c
*/
char *get_section_name(Elf64_Ehdr *ehead,
	Elf64_Shdr *rshead, Elf64_Shdr *shead);
Elf64_Shdr *elf_get_sheader(Elf64_Ehdr *hdr);
Elf64_Shdr *elf_get_section(info_nm_t *info, char const *sname);
int check_elf(info_nm_t *info);
int print_elf(info_nm_t *info);

/*
**ar.c
*/
int print_ar(info_nm_t *info);
int check_ar(info_nm_t *info);

/*
** extract_symbol.c
*/
int extract_symbol_list(info_nm_t *infos);
int map_file(info_file_t *finfo);

/*
**misc.c
*/
int nm_perror(info_nm_t *infos);
int nmputerror(info_nm_t *infos, char const *str);
int is_nm_printable(size_t x);

/*
** str.c
*/
int my_strlen(void *ptr, char c);
int spec_strcmp(char *a, char *b);

/*
**liked_list.c
*/
void list_clear(list_t *list);
int list_add(list_t *front_ptr, void *elem);
void list_sort(list_t list);

#endif /* !NMOBJDUMP_NM_H */
