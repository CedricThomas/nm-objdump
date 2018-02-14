/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** str.c
*/
#include <glob.h>
#include <ctype.h>
#include "nm.h"

int my_strlen(void *ptr, char c)
{
	char *str = ptr;
	int i = -1;

	while (str[++i] != c && str[i]);
	return (i);
}

int spec_strcmp(char *a, char *b)
{
	size_t i = 0;
	size_t j = 0;

	while (a[i] && b[j]) {
		while (a[i] == '_')
			i++;
		while (b[j] == '_')
			j++;
		if (tolower(a[i]) != tolower(b[j]))
			break;
		i += 1;
		j += 1;
	}
	return tolower(a[i]) - tolower(b[j]);
}