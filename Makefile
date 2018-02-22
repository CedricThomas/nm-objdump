##
## EPITECH PROJECT, 2018
## nm/objdump
## File description:
## Epitech project
##

NM_SRC		=	main.c			\
			nm.c			\
			linked_list.c		\
			misc.c			\
			str.c			\
			ar.c			\
			elf.c


OBJDUMP_SRC	=	main.c			\
			obj.c			\
			ar.c			\
			elf.c			\
			misc.c			\
			print_hexa.c		\
			print.c

NM_OBJ		=	$(addprefix ./nm/, $(NM_SRC:.c=.o))

OBJDUMP_OBJ	=	$(addprefix ./objdump/, $(OBJDUMP_SRC:.c=.o))

NM_NAME		=	my_nm

OBJDUMP_NAME	=	my_objdump

$(NM_NAME): CFLAGS = -g -W -Wall -Wextra -I nm/include

$(OBJDUMP_NAME): CFLAGS	= -g -W -Wall -Wextra  -I objdump/include

all: nm objdump

nm: $(NM_NAME)

$(NM_NAME): $(NM_OBJ)
	gcc -c $(CFLAGS) -DARCHI64 $(addprefix ./nm/, elf_format.c) -o $(addprefix ./nm/, elf_format_64.o)
	gcc -c $(CFLAGS) -DARCHI32 $(addprefix ./nm/, elf_format.c) -o $(addprefix ./nm/, elf_format_32.o)

	gcc -c $(CFLAGS) -DARCHI64 $(addprefix ./nm/, print_type.c) -o $(addprefix ./nm/, print_type_64.o)
	gcc -c $(CFLAGS) -DARCHI32 $(addprefix ./nm/, print_type.c) -o $(addprefix ./nm/, print_type_32.o)

	gcc -c $(CFLAGS) -DARCHI64 $(addprefix ./nm/, symbols.c) -o $(addprefix ./nm/, symbols_64.o)
	gcc -c $(CFLAGS) -DARCHI32 $(addprefix ./nm/, symbols.c) -o $(addprefix ./nm/, symbols_32.o)

	gcc -o $(NM_NAME) $(NM_OBJ)\
			$(addprefix ./nm/, elf_format_64.o) $(addprefix ./nm/, elf_format_32.o)	\
			$(addprefix ./nm/, print_type_64.o) $(addprefix ./nm/, print_type_32.o)	\
			$(addprefix ./nm/, symbols_64.o) $(addprefix ./nm/, symbols_32.o)

objdump: $(OBJDUMP_NAME)

$(OBJDUMP_NAME): $(OBJDUMP_OBJ)
	gcc -c $(CFLAGS) -DARCHI64 $(addprefix ./objdump/, elf_format.c) -o $(addprefix ./objdump/, elf_format_64.o)
	gcc -c $(CFLAGS) -DARCHI32 $(addprefix ./objdump/, elf_format.c) -o $(addprefix ./objdump/, elf_format_32.o)

	gcc -c $(CFLAGS) -DARCHI64 $(addprefix ./objdump/, common.c) -o $(addprefix ./objdump/, common_64.o)
	gcc -c $(CFLAGS) -DARCHI32 $(addprefix ./objdump/, common.c) -o $(addprefix ./objdump/, common_32.o)

	gcc -o $(OBJDUMP_NAME) $(OBJDUMP_OBJ) \
			$(addprefix ./objdump/, elf_format_64.o) $(addprefix ./objdump/, elf_format_32.o)\
			$(addprefix ./objdump/, common_64.o) $(addprefix ./objdump/, common_32.o)

clean:
	rm -rf $(OBJDUMP_OBJ)
	rm -rf $(NM_OBJ)

fclean: clean
	rm -rf $(OBJDUMP_NAME)
	rm -rf $(NM_NAME)

re: fclean all

.PHONY: all clean fclean re nm objdump
