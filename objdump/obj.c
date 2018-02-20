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

static int map_file(info_file_t *finfo)
{
	struct stat s;
	int fd;

	fd = open(finfo->name, O_RDONLY);
	if (fd == -1)
		return (1);
	if (fstat(fd, &s) == -1)
		return (1);
	finfo->size = (size_t) s.st_size;
	finfo->vadress = mmap(0, finfo->size, PROT_READ,
	                      MAP_PRIVATE, fd, 0);
	if (finfo->vadress == MAP_FAILED)
		return (1);
	close(fd);
	return (0);
}

int objdump(info_obj_t *info)
{
	int format = 0;

	if (map_file(&info->finfo))
		return (my_perror(info));
	format = check_elf(info);
	if (format == SUCCESS) {
		if (print_elf(&info->finfo))
			return (ERROR);
	} else if (check_ar(info) == SUCCESS) {
		if (print_ar(info))
			return (ERROR);
	} else
		return (my_puterror(info, "Invalid elf header."));
	munmap(info->finfo.vadress, info->finfo.size);
	return (0);
}
