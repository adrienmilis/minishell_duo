SRCS = parser.c \
	utils.c \
	errors.c \
	linked_list.c \
	libft.c \
	add_arguments.c \
	check_chars.c \
	check_syntax.c \
	parse_out_quotes.c \
	env.c \
	env_vars.c \
	simple_quotes.c \
	check_chars2.c \
	check_chars3.c \
	double_quotes.c \
	out_quotes_utils.c \
	signs.c

SRCS_PARS = parser.c \
	utils.c \
	main.c \
	errors.c \
	linked_list.c \
	libft.c \
	add_arguments.c \
	check_chars.c \
	check_syntax.c \
	parse_out_quotes.c \
	env.c \
	env_vars.c \
	simple_quotes.c \
	check_chars2.c \
	check_chars3.c \
	dupl_fcts.c \
	double_quotes.c \
	out_quotes_utils.c \
	signs.c

OBJS = $(SRCS:.c=.o)
NAME = parser.a
FLAGS = -Wall -Wextra -Werror -c

OBJS_PARS = $(SRCS_PARS:.c=.o)
NAME_PARS = minishell
FLAGS_PARS = -Wall -Wextra -Werror

all: $(NAME)

$(OBJS): $(SRCS)
	gcc $(FLAGS) $(SRCS)

$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)

clean:
	rm -rf $(OBJS)

pars: $(OBJS_PARS)
	gcc $(FLAGS_PARS) $(OBJS_PARS) -o $(NAME_PARS)
