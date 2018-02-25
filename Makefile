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

NM_FOLDER	=	./new_nm/

OBJDUMP_FOLDER	=	./objdump/

NM_OBJ		=	$(addprefix $(NM_FOLDER), $(NM_SRC:.c=.o))

OBJDUMP_OBJ	=	$(addprefix $(OBJDUMP_FOLDER), $(OBJDUMP_SRC:.c=.o))

NM_NAME		=	my_nm

OBJDUMP_NAME	=	my_objdump

$(NM_NAME): CFLAGS = -g -W -Wall -Wextra -I $(addprefix $(NM_FOLDER), include)

$(OBJDUMP_NAME): CFLAGS	= -g -W -Wall -Wextra  -I  $(addprefix $(OBJDUMP_FOLDER), include)

all: nm objdump

nm: $(NM_NAME)

$(NM_NAME): $(NM_OBJ)
	gcc -c $(CFLAGS) -DARCHI64 $(addprefix $(NM_FOLDER), elf_format.c) -o $(addprefix $(NM_FOLDER), elf_format_64.o)
	gcc -c $(CFLAGS) -DARCHI32 $(addprefix $(NM_FOLDER), elf_format.c) -o $(addprefix $(NM_FOLDER), elf_format_32.o)

	gcc -c $(CFLAGS) -DARCHI64 $(addprefix $(NM_FOLDER), print_type.c) -o $(addprefix $(NM_FOLDER), print_type_64.o)
	gcc -c $(CFLAGS) -DARCHI32 $(addprefix $(NM_FOLDER), print_type.c) -o $(addprefix $(NM_FOLDER), print_type_32.o)

	gcc -c $(CFLAGS) -DARCHI64 $(addprefix $(NM_FOLDER), symbols.c) -o $(addprefix $(NM_FOLDER), symbols_64.o)
	gcc -c $(CFLAGS) -DARCHI32 $(addprefix $(NM_FOLDER), symbols.c) -o $(addprefix $(NM_FOLDER), symbols_32.o)

	gcc -o $(NM_NAME) $(NM_OBJ)\
			$(addprefix $(NM_FOLDER), elf_format_64.o) $(addprefix $(NM_FOLDER), elf_format_32.o)	\
			$(addprefix $(NM_FOLDER), print_type_64.o) $(addprefix $(NM_FOLDER), print_type_32.o)	\
			$(addprefix $(NM_FOLDER), symbols_64.o) $(addprefix $(NM_FOLDER), symbols_32.o)

objdump: $(OBJDUMP_NAME)

$(OBJDUMP_NAME): $(OBJDUMP_OBJ)
	gcc -c $(CFLAGS) -DARCHI64 $(addprefix $(OBJDUMP_FOLDER), elf_format.c) -o $(addprefix $(OBJDUMP_FOLDER), elf_format_64.o)
	gcc -c $(CFLAGS) -DARCHI32 $(addprefix $(OBJDUMP_FOLDER), elf_format.c) -o $(addprefix $(OBJDUMP_FOLDER), elf_format_32.o)

	gcc -c $(CFLAGS) -DARCHI64 $(addprefix $(OBJDUMP_FOLDER), common.c) -o $(addprefix $(OBJDUMP_FOLDER), common_64.o)
	gcc -c $(CFLAGS) -DARCHI32 $(addprefix $(OBJDUMP_FOLDER), common.c) -o $(addprefix $(OBJDUMP_FOLDER), common_32.o)

	gcc -o $(OBJDUMP_NAME) $(OBJDUMP_OBJ) \
			$(addprefix $(OBJDUMP_FOLDER), elf_format_64.o) $(addprefix $(OBJDUMP_FOLDER), elf_format_32.o)\
			$(addprefix $(OBJDUMP_FOLDER), common_64.o) $(addprefix $(OBJDUMP_FOLDER), common_32.o)

clean:
	rm -rf $(OBJDUMP_OBJ)
	rm -rf $(NM_OBJ)

fclean: clean
	rm -rf $(OBJDUMP_NAME)
	rm -rf $(NM_NAME)

re: fclean all

.PHONY: all clean fclean re nm objdump
