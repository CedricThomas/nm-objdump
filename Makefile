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
			extract_symbol.c	\
			elf.c

OBJDUMP_SRC	=	main.c

NM_OBJ		=	$(addprefix ./nm/, $(NM_SRC:.c=.o))

OBJDUMP_OBJ	=	$(addprefix ./objdump/, $(OBJDUMP_SRC:.c=.o))

NM_NAME		=	my_nm

OBJDUMP_NAME	=	my_objdump

$(NM_NAME): CFLAGS = -g -W -Wall -Wextra -I nm/include

$(OBJDUMP_NAME): CFLAGS	= -g -W -Wall -Wextra  -I objdump/include

all: $(NM_NAME) $(OBJDUMP_NAME)

$(NM_NAME): $(NM_OBJ)
	gcc -o $(NM_NAME) $(NM_OBJ)

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
