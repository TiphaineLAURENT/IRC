##Press Ctrl-C+H then enter this file name and press enter

CC		=	gcc

RM		=	rm -f

CFLAGS		=	-W -Wall -Wextra -Wshadow -Wunreachable-code -Wconversion -Wstrict-prototypes
CFLGAS		+=	-Werror
CFLAGS		+=	-Wswitch-default -Wswitch-enum
CFLAGS		+=	-Wuninitialized -Winit-self
CFLAGS		+=	-fstack-protector-strong
CFLAGS		+=	-pedantic
CFLAGS		+=	-march=native
CPPFLAGS	=	-I include

UTIL_DIR	=	util
UTIL_SRC	=	\

SRC_DIR		=	src
SRC_FILES	=	$(addprefix $(UTIL_DIR)/, $(UTIL_SRC))	\
			main.c					\

SRCS		=	$(addprefix $(SRC_DIR)/, $(SRC_FILES))

OBJS		=	$(SRCS:.c=.o)

NAME		=	client

all:		$(NAME)

$(NAME):	$(OBJS)
		$(CC) -o $(NAME) $(OBJS)

clean:
		$(RM) $(OBJS)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all

.PHONY:		all clean fclean re
