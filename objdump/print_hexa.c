/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** print_hexa.c
*/
#include <glob.h>
#include <stdio.h>
#include <elf.h>
#include "objdump.h"

static void print_hex_val(unsigned char *section, size_t hsize, size_t idx)
{
	size_t i;

	for (i = idx; i < idx + BYTES && i < hsize; i++) {
		if ((i - idx) % COL == 0)
			printf(" ");
		printf("%02x", section[i]);
	}
	while (i < idx + BYTES) {
		if ((i - idx) % COL == 0)
			printf(" ");
		printf("  ");
		i++;
	}
}

static void print_ascii_val(unsigned char *section, size_t hsize, size_t idx)
{
	size_t i;

	for (i = idx; i < idx + BYTES && i < hsize; i++) {
		if (section[i] >= ' ' && section[i] <= '~')
			printf("%c", section[i]);
		else
			printf(".");
	}
	while (i < idx + BYTES) {
		printf(" ");
		i++;
	}
}

int print_hexa_64(unsigned char *section, Elf64_Shdr *sheader)
{
	for (size_t idx = 0; idx < sheader->sh_size; idx += BYTES) {
		printf(" %04lx", sheader->sh_addr + idx);
		print_hex_val(section, sheader->sh_size, idx);
		printf("  ");
		print_ascii_val(section, sheader->sh_size, idx);
		printf("\n");
	}
	return (0);
}

int print_hexa_32(unsigned char *section, Elf32_Shdr *sheader)
{
	for (size_t idx = 0; idx < sheader->sh_size; idx += BYTES) {
		printf(" %04lx", sheader->sh_addr + idx);
		print_hex_val(section, sheader->sh_size, idx);
		printf("  ");
		print_ascii_val(section, sheader->sh_size, idx);
		printf("\n");
	}
	return (0);
}
