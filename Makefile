CC=gcc
SRC=main.c
OBJ=$(SRC:.c=.o)
NAME=fdf
FLAGS=-Wall -Wextra -Werror
MLXFLAGS=-lmlx -framework OpenGL -framework AppKit
LIBFLAGS=-lft -L../libft
MATH=-lm
DEPS=fdf.h

.PHONY: all debug clean fclean re

all: $(NAME)

$(OBJ): $(SRC) $(DEPS)
	gcc $(FLAGS) -c $(SRC) -o $(OBJ)

$(NAME): $(OBJ)
	gcc $(FLAGS) $(OBJ) -o $(NAME) $(MLXFLAGS) $(MATH) $(LIBFLAGS)

debug:
	gcc -ggdb3 $(FLAGS) -c $(SRC) -o $(OBJ)
	gcc $(FLAGS) $(OBJ) -o $(NAME) $(MLXFLAGS) $(MATH) $(LIBFLAGS)
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
