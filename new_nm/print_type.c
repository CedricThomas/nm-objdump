/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** print_type.c
*/
#include <stdlib.h>
#include <elf.h>
#include <memory.h>
#include "nm.h"

#ifdef ARCHI32
static char get_bind_symbol_32(Elf_Sym *sym, char *c)
#elif ARCHI64
static char get_bind_symbol_64(Elf_Sym *sym, char *c)
#endif
{
	*c = '?';
	if (ELF64_ST_BIND(sym->st_info) == STB_GNU_UNIQUE)
		*c = 'u';
	else if (ELF64_ST_BIND(sym->st_info) == STB_WEAK) {
		*c = 'W';
		if (sym->st_shndx == SHN_UNDEF)
			*c = 'w';
	} else if (ELF64_ST_BIND(sym->st_info) == STB_WEAK &&
	           ELF64_ST_TYPE(sym->st_info) == STT_OBJECT) {
		*c = 'V';
		if (sym->st_shndx == SHN_UNDEF)
			*c = 'v';
	}
	return (SUCCESS);
}


#ifdef ARCHI32
static char get_progbits_32(Elf_Shdr *binded, char *c)
#elif ARCHI64
static char get_progbits_64(Elf_Shdr *binded, char *c)
#endif
{
	*c = '?';
	if (binded->sh_type == SHT_PROGBITS
	         && binded->sh_flags == SHF_ALLOC)
		*c = 'R';
	else if (binded->sh_type == SHT_PROGBITS &&
		binded->sh_flags == (SHF_ALLOC | SHF_WRITE))
		*c = 'D';
	else if (binded->sh_type == SHT_PROGBITS &&
		binded->sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		*c = 'T';
	return (SUCCESS);
}

#ifdef ARCHI32
static char get_special_32(Elf_Shdr *binded, char *c)
#elif ARCHI64
static char get_special_64(Elf_Shdr *binded, char *c)
#endif
{
	*c = '?';
	if (binded->sh_type == SHT_NOBITS &&
		binded->sh_flags == (SHF_ALLOC | SHF_WRITE))
		*c = 'B';
	else if (binded->sh_type == SHT_DYNAMIC)
		*c = 'D';
	return (SUCCESS);
}

#ifdef ARCHI32
static int get_basic_32(Elf_Sym *sym, char *c)
#elif ARCHI64
static int get_basic_64(Elf_Sym *sym, char *c)
#endif
{
	*c = '?';
	switch (sym->st_shndx) {
		case SHN_UNDEF:
			*c = 'U';
			break;
		case SHN_ABS:
			*c = 'A';
			break;
		case SHN_COMMON:
			*c = 'C';
			break;
		default:
			*c = '?';
			break;
	}
	return (SUCCESS);
}

#ifdef ARCHI32
char print_type_32(info_nm_t *info, node_t *n)
#elif ARCHI64
char print_type_64(info_nm_t *info, node_t *n)
#endif
{
	int sum = 0;
	#ifdef ARCHI32
	Elf_Shdr *shdr = elf_get_sheader_32(info);
	Elf_Sym *s = n->value;

	get_bind_symbol_32(s, &n->type);
	if (n->type == '?' && get_basic_32(s, &n->type) == ERROR)
		return (nmputerror(info, "File truncated"));
	if (n->type == '?') {
		if (out_of_map(info, shdr + s->st_shndx, sizeof(Elf_Shdr)))
			return (nmputerror(info, "File truncated"));
		else if (get_progbits_32(&shdr[s->st_shndx], &n->type) == ERROR)
			return (nmputerror(info, "File truncated"));
	}
	if (n->type == '?' &&
		get_special_32(&shdr[s->st_shndx], &n->type) == ERROR)
		return (nmputerror(info, "File truncated"));
	#elif ARCHI64
	Elf_Shdr *shdr = elf_get_sheader_64(info);
	Elf_Sym *s = n->value;

	get_bind_symbol_64(s, &n->type);
	if (n->type == '?' && get_basic_64(s, &n->type) == ERROR)
		return (nmputerror(info, "File truncated"));
	if (n->type == '?') {
		if (out_of_map(info, shdr + s->st_shndx, sizeof(Elf_Shdr)))
			return (nmputerror(info, "File truncated"));
		else if (get_progbits_64(&shdr[s->st_shndx], &n->type) == ERROR)
			return (nmputerror(info, "File truncated"));
	}
	if (n->type == '?' &&
		get_special_64(&shdr[s->st_shndx], &n->type) == ERROR)
		return (nmputerror(info, "File truncated"));
	#endif
	sum = (ELF64_ST_BIND(s->st_info) == STB_LOCAL && n->type != '?') * 32;
	n->type += sum;
	return (SUCCESS);
}
