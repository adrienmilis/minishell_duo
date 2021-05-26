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
	check_chars3.c

OBJS = $(SRCS:.c=.o)
NAME = parser.a
FLAGS = -Wall -Wextra -Werror -c

all: $(NAME)

$(OBJS): $(SRCS)
	gcc $(FLAGS) $(SRCS)

$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)
	ranlib $(NAME)

clean:
	rm -rf $(OBJS)