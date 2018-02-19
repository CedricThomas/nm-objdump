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
			flags.c			\
			obj.c			\
			ar.c			\
			elf.c			\
			elf_format.c		\
			misc.c			\
			print_hexa.c		\
			print_header.c		\
			print_section.c

NM_OBJ		=	$(addprefix ./nm/, $(NM_SRC:.c=.o))

OBJDUMP_OBJ	=	$(addprefix ./objdump/, $(OBJDUMP_SRC:.c=.o))

NM_NAME		=	my_nm

OBJDUMP_NAME	=	my_objdump

$(NM_NAME): CFLAGS = -g -W -Wall -Wextra -I nm/include

$(OBJDUMP_NAME): CFLAGS	= -g -W -Wall -Wextra  -I objdump/include

all: $(NM_NAME) $(OBJDUMP_NAME)

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

$(OBJDUMP_NAME): $(OBJDUMP_OBJ)
	gcc -o $(OBJDUMP_NAME) $(OBJDUMP_OBJ)

clean:
	rm -rf $(OBJDUMP_OBJ)
	rm -rf $(NM_OBJ)

fclean: clean
	rm -rf $(OBJDUMP_NAME)
	rm -rf $(NM_NAME)

re: fclean all

.PHONY: all clean fclean re
