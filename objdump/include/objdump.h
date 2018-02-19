/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** objdump.h
*/

#ifndef NMOBJDUMP_OBJDUMP_H
	#define NMOBJDUMP_OBJDUMP_H

	#include <elf.h>

	#define ERROR 84
	#define HEADER_ERROR 42
	#define SUCCESS 0
	#define BYTES 16
	#define COL 4

typedef struct info_file_s {
	char *name;
	void *vadress;
	size_t size;
	size_t archi;
} info_file_t;

typedef struct info_obj_s {
	info_file_t finfo;
	char *name;
} info_obj_t;

/*
**obj.c
*/
int objdump(info_obj_t *infos);

/*
**elf_format.c
*/
char *get_section_name_64(Elf64_Ehdr *ehead, Elf64_Shdr *rshead,
	Elf64_Shdr *shead);
Elf64_Shdr *elf_get_sheader_64(Elf64_Ehdr *hdr);

char *get_section_name_32(Elf32_Ehdr *ehead, Elf32_Shdr *rshead,
	Elf32_Shdr *shead);
Elf32_Shdr *elf_get_sheader_32(Elf32_Ehdr *hdr);

/*
**elf.c
*/
int check_elf(info_obj_t *info);
int print_elf(info_file_t *info);

/*
**ar.c
*/
int check_ar(info_obj_t *info);
int print_ar(info_obj_t *info);

/*
**misc.c
*/
int my_perror(info_obj_t *infos);
int my_puterror(info_obj_t *infos, char const *str);
int my_strlen(void *ptr, char c);
void print_flags(size_t flags);

/*
**print_section.c
*/
int print_sections(info_file_t *info);

/*
**print_header.c
*/
int print_header_32(info_file_t *info);
int print_header_64(info_file_t *info);
int print_header(info_file_t *info);

/*
**flags.c
*/
size_t catch_flags_64(Elf64_Shdr *shdr, Elf64_Ehdr *ehdr,
	size_t size_headers);
size_t catch_flags_32(Elf32_Shdr *shdr, Elf32_Ehdr *ehdr,
	size_t size_headers);

/*
**print_hexa.c
*/
int print_hexa(unsigned char *section, size_t addr, size_t  size);

#endif /* !NMOBJDUMP_OBJDUMP_H */
