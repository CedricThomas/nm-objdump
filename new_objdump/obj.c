/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** obj.c
*/
#include <glob.h>
#include <zconf.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "objdump.h"


static int map_file(info_obj_t *i)
{
	info_file_t *finfo = &i->finfo;
	struct stat s;
	int fd;

	if (access(finfo->name, F_OK) == -1)
		return (my_specputerror(i, "No such file"));
	fd = open(finfo->name, O_RDONLY);
	if (fd == -1)
		return (my_puterror(i, "Permission denied"));
	if (fstat(fd, &s) == -1)
		return (my_puterror(i, "Permission denied"));
	finfo->size = (size_t) s.st_size;
	finfo->vadress = mmap(0, finfo->size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (finfo->vadress == MAP_FAILED)
		return (my_puterror(i, "not enought memory"));
	close(fd);
	return (SUCCESS);
}

int objdump(info_obj_t *info)
{
	int ret = ERROR;

	if (map_file(info) == ERROR)
		return (ERROR);
	if (check_ar(info) == SUCCESS) {
		ret = SUCCESS;
		if (print_ar(info))
			return (my_munmap(info, ERROR));
	} else if (check_elf(info) == SUCCESS) {
		ret = SUCCESS;
		if (print_elf(info))
			return (my_munmap(info, ERROR));
	}
	return (my_munmap(info, ret));
}
