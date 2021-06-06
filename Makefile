NAME = minishell

SRCS = new_megashell.c \
		minishell_utils.c \
		list_utils.c \
		minishell_utils2.c \
		msh_misc.c \
		msh_main_loop.c \

OBJS = $(SRCS:.c=.o)

FLAGS = -Wall -Wextra -Werror -lncurses

CC = gcc

$(NAME): $(OBJS)
	make -C parser
	make -C execution
	gcc $(FLAGS) -g3 parser/parser.a execution/execution.a $(OBJS) -o $(NAME)
	make clean -C parser
	make clean -C execution

all: $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

norm:
	norminette $(SRCS)
