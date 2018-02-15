/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** str.c
*/
#include <glob.h>
#include <ctype.h>
#include "nm.h"

static int is_in(char c, char *str)
{
	for (int i = 0; str[i]; i++)
		if (str[i] == c)
			return (1);
	return (0);
}

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
		while (is_in(a[i], "_."))
			i++;
		while (is_in(b[j], "_."))
			j++;
		if (tolower(a[i]) != tolower(b[j]))
			break;
		i += 1;
		j += 1;
	}
	if (tolower(a[i]) - tolower(b[j]) == 0)
		return (int) (j - i);
	return tolower(a[i]) - tolower(b[j]);
}