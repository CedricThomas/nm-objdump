/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** linked_list.c
*/
#include <zconf.h>
#include <malloc.h>
#include <memory.h>
#include "nm.h"

void list_clear(list_t *list)
{
	list_t old_tmp = NULL;
	list_t tmp = *list;

	while (tmp) {
		old_tmp = tmp;
		tmp = tmp->next;
		free(old_tmp);
	}
	*list = NULL;
}

int list_add(list_t *front_ptr, void *elem)
{
	list_t new;

	new = malloc(sizeof(node_t));
	if (new == 0)
		return (0);
	memset(new, 0, sizeof(node_t));
	new->next = *front_ptr;
	new->value = elem;
	*front_ptr = new;
	return (1);
}

static void swap_infos(list_t a, list_t b)
{
	void *tmp_value = a->value;
	void *tmp_name = a->name;

	a->value = b->value;
	a->name = b->name;
	b->value = tmp_value;
	b->name = tmp_name;
}

static void try_swap(list_t tmp1, list_t tmp2, int *sorted)
{
	if (spec_strcmp(tmp1->name, tmp2->name) > 0) {
		*sorted = 1;
		swap_infos(tmp1, tmp2);
	}
}

void list_sort(list_t list)
{
	int sorted = 1;
	list_t tmp1;
	list_t tmp2;

	while (sorted) {
		sorted = 0;
		tmp1 = list;
		tmp2 = list;
		if (tmp2)
			tmp2 = tmp2->next;
		while (tmp1 && tmp2) {
			try_swap(tmp1, tmp2, &sorted);
			tmp1 = tmp1->next;
			tmp2 = tmp2->next;
		}
	}
}