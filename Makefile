CC			:=	cc
CFLAGS		:=	-MMD -Wall -Werror -Wextra
CFLAGSADD	:=	-g3 -pthread #-fsanitize=thread

IPATH		:=	.

SRCS		:=	main.c\
				fork.c\
				utils.c\
				routine.c\
				parsing.c\
				init_philo.c\
				loop.c

NAME		:=	philo

OBJ			=	$(SRCS:.c=.o)
DEP			=	$(OBJ:.o=.d)

all			:	$(NAME)

%.o:			%.c
				$(CC) $(CFLAGS) $(CFLAGSADD) -I $(IPATH) -c $< -o $@

$(NAME)			:	$(OBJ)
				$(CC) $(CFLAGS) $(CFLAGSADD) $(OBJ) -I $(IPATH) -o $@

clean		:
				rm -f $(OBJ)
				rm -f $(DEP)

fclean		:
				make clean
				rm -f $(NAME)

re			:
				make fclean
				make all

-include $(DEP)

.PHONY		:	all clean fclean re
