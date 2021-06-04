NAME	=	minishell

SRCS = new_megashell.c \
		minishell_utils.c \
		list_utils.c \
		minishell_utils2.c \
		msh_misc.c \
		msh_main_loop.c

all:
	make -C parser
	make -C execution
	gcc -Wall -Wextra -Werror -g3 -lncurses parser/parser.a execution/execution.a $(SRCS) -o $(NAME)
	make clean -C parser
	make fclean -C execution
