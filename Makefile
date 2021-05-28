NAME	=	minishell

SRCS = new_megashell.c \
		minishell_utils.c \
		list_utils.c

all:
	make -C parser
	make -C execution
	gcc -Wall -Wextra -Werror -g -lncurses parser/parser.a execution/execution.a $(SRCS) -o $(NAME)
	make clean -C parser
	make fclean -C execution
