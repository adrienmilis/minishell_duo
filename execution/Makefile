NAME		=	execution.a

SRCS		=	execution.c \
				myenv.c \
				ft_split.c \
				utils.c

OBJS		=	$(SRCS:.c=.o)

OBJS_BONUS	=	$(SRCS_BONUS:.c=.o)

CC			=	gcc

CFLAGS		=	-Wall -Wextra -Werror

RM			=	rm -f

.c.o:
			$(CC) $(CFLAGS) -g3 -c $< -o $(<:.c=.o)

$(NAME):	$(OBJS)
			ar rc $@ $^

all:		$(NAME)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
