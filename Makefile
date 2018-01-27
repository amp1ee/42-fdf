CC=gcc
SRC=main.c controls.c read.c draw.c color.c util.c
OBJ=$(SRC:.c=.o)
NAME=fdf
FLAGS=-Wall -Wextra -Werror
OS_NAME= $(shell uname -s)
MLXFLAGS=-lmlx
LIBFLAGS=-lft -L../libft
LIBFT=../libft/libft.a
MATH=-lm
DEPS=fdf.h

.PHONY: all debug clean fclean re

ifeq ($(OS_NAME), Linux)
MLXFLAGS += -lXext -lX11
else
MLXFLAGS += -framework OpenGL -framework AppKit
endif

all: $(NAME)

$(OBJ): $(SRC) $(DEPS)
	gcc $(FLAGS) -c $(SRC)

$(NAME): $(OBJ) $(LIBFT)
	gcc $(OBJ) -o $(NAME) $(MLXFLAGS) $(MATH) $(LIBFLAGS)

$(LIBFT):
	make -C ../libft/

debug:
	gcc -ggdb3 $(FLAGS) -c $(SRC) -o $(OBJ)
	gcc $(OBJ) -o $(NAME) $(MLXFLAGS) $(MATH) $(LIBFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
