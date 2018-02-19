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

	#ifdef ARCHI64
		typedef Elf64_Sym Elf_Sym;
		typedef Elf64_Ehdr Elf_Ehdr;
		typedef Elf64_Shdr Elf_Shdr;
	#elif ARCHI32
		typedef Elf32_Sym Elf_Sym;
		typedef Elf32_Ehdr Elf_Ehdr;
		typedef Elf32_Shdr Elf_Shdr;
	#endif /* !ARCHI */

typedef struct node_s {
	char *name;
	void *value;
	struct node_s *next;
} node_t;

typedef node_t *list_t;

typedef struct info_file_s {
	char *name;
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
int extract_symbol_list(info_nm_t *infos);

/*
**elf_format.c
*/
char *get_section_name_64(Elf64_Ehdr *ehead,
	Elf64_Shdr *rshead, Elf64_Shdr *shead);
Elf64_Shdr *elf_get_sheader_64(Elf64_Ehdr *hdr);
Elf64_Shdr *elf_get_section_64(info_file_t *info, char const *sname);

char *get_section_name_32(Elf32_Ehdr *ehead,
	Elf32_Shdr *rshead, Elf32_Shdr *shead);
Elf32_Shdr *elf_get_sheader_32(Elf32_Ehdr *hdr);
Elf32_Shdr *elf_get_section_32(info_file_t *info, char const *sname);

/*
**elf.c
*/
int check_elf(info_nm_t *info);
int print_elf(info_nm_t *info);

/*
**ar.c
*/
int print_ar(info_nm_t *info);
int check_ar(info_nm_t *info);

/*
** symbols.c
*/
int store_symbol_list_32(info_nm_t *infos, uint32_t size,
	Elf32_Sym *symbols, void *strtab_p);
int store_symbol_list_64(info_nm_t *infos, uint64_t size,
	Elf64_Sym *symbols, void *strtab_p);
int create_symbol_list_32(info_nm_t *infos);
int create_symbol_list_64(info_nm_t *infos);
void print_syms_32(info_file_t *info);
void print_syms_64(info_file_t *info);

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

/*
**print_type.c
*/
char print_type_64(Elf64_Sym *sym, info_file_t *info);
char print_type_32(Elf32_Sym *sym, info_file_t *info);

#endif /* !NMOBJDUMP_NM_H */
