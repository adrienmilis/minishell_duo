NAME = minishell

SRCS = new_megashell.c \
		minishell_utils.c \
		list_utils.c \
		minishell_utils2.c \
		msh_misc.c \
		msh_main_loop.c \

OBJS = $(SRCS:.c=.o)

FLAGS = -Wall -Wextra -Werror

CC = gcc

.c.o:
	$(CC) $(FLAGS) -c $< -o $(<:.c=.o)

$(NAME): $(OBJS)
	make -C parser
	make -C execution
	$(CC) $(FLAGS) -lncurses parser/parser.a execution/execution.a $(OBJS) -o $(NAME)
	make clean -C parser
	make clean -C execution

all: $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)
	make fclean -C parser
	make fclean -C execution

re: fclean all

norm:
	make norm -C parser
	make norm -C execution
	norminette $(SRCS)

.PHONY:	all clean fclean re norm
