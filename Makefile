CC=gcc
SRC=main.c
OBJ=$(SRC:.c=.o)
NAME=fdf
FLAGS=-Wall -Wextra -Werror
MLXFLAGS=-lmlx -lXext -lX11
LIBFLAGS=-lft -L../libft
MATH=-lm

.PHONY: all clean fclean re

all: $(NAME)

$(OBJ): $(SRC)
	gcc $(FLAGS) $(MLXFLAGS) $(LIBFLAGS) $(MATH) -c $(SRC) -o $(OBJ)

$(NAME): $(OBJ)
	gcc $(FLAGS) $(OBJ) -o $(NAME) $(MLXFLAGS) $(MATH) $(LIBFLAGS)
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
