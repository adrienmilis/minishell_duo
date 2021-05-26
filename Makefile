NAME	=	minishell

all:
	make -C parser
	make -C execution
	gcc -Wall -Wextra -Werror -lncurses parser/parser.a execution/execution.a megashell.c -o $(NAME)
	make clean -C parser
	make clean -C execution
