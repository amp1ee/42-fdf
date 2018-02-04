CC			= gcc
SRC			= main.c controls.c read.c draw.c color.c util.c readutils.c
OBJ			= $(SRC:.c=.o)
NAME		= fdf
FLAGS		= -Wall -Wextra -Werror
OS_NAME		= $(shell uname -s)
MLXFLAGS	= -lmlx
LIBFLAGS	= -lft -L../libft
LIBFT		= ../libft/libft.a
MATH		= -lm
INCL		= fdf.h readutils.h keyb_{linux, mac}.h
INCL_LIB	= ./includes/

.PHONY: all debug clean fclean re

ifeq ($(OS_NAME), Linux)
MLXFLAGS += -lXext -lX11
else
MLXFLAGS += -framework OpenGL -framework AppKit
endif

all: $(NAME)

$(OBJ): $(SRC) $(INCL_LIB)fdf.h
	gcc $(FLAGS) -c $(SRC) -I$(INCL_LIB)

$(NAME): $(OBJ) $(LIBFT)
	gcc $(OBJ) -o $(NAME) $(MLXFLAGS) $(MATH) $(LIBFLAGS)

$(LIBFT):
	make -C ../libft/

debug: $(SRC) $(DEPS)
	gcc -ggdb3 $(FLAGS) -c $(SRC) -I$(INCL_LIB)
	gcc $(OBJ) -o $(NAME) $(MLXFLAGS) $(MATH) $(LIBFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
