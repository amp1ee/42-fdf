NAME		= fdf
CC			= gcc
SRCDIR		= ./src/
OBJDIR		= ./obj/
HDIR		= ./includes/
LIBDIR		= ./libft/
SRC			= $(addprefix $(SRCDIR), \
				main.c \
				controls.c \
				read.c \
				draw.c \
				color.c \
				util.c \
				readutils.c)
OBJ			= $(SRC:$(SRCDIR)%.c=$(OBJDIR)%.o)
FLAGS		= -Wall -Wextra -Werror
MLXFLAGS	= -lmlx
LIBFLAGS	= -lft -L$(LIBDIR)
MATH		= -lm
OS_NAME		= $(shell uname -s)
ifeq ($(OS_NAME), Linux)
MLXFLAGS 	+= -lXext -lX11
else
MLXFLAGS 	+= -framework OpenGL -framework AppKit
endif
LIBFT		= $(LIBDIR)libft.a
INCL		= fdf.h readutils.h util.h keyb_{linux, mac}.h
FDF_H		= $(HDIR)fdf.h

.PHONY: all debug clean fclean re

all: $(NAME)

$(OBJDIR)%.o: $(SRCDIR)%.c $(FDF_H)
	@mkdir -p $(OBJDIR)
	@printf '\tCompiling $<\n'
	@gcc $(FLAGS) -c $< -o $@ -I$(HDIR)
$(NAME): $(OBJ) $(LIBFT)
	@printf '\tLinking\n'
	@gcc $(OBJ) -o $(NAME) $(MLXFLAGS) $(MATH) $(LIBFLAGS)
	@echo 'Done'
$(LIBFT):
	@printf '\tAssembling libft\n'
	@make -sC $(LIBDIR)

debug: $(SRC) $(FDF_H)
	@gcc -ggdb3 $(FLAGS) -c $(SRC) -I$(HDIR)
	@gcc $(OBJ) -o $(NAME) $(MLXFLAGS) $(MATH) $(LIBFLAGS)

clean:
	@make -sC $(LIBDIR) clean
	@rm -rf $(OBJDIR)
	@printf '\tObject files deleted\n'
fclean: clean
	@rm -f $(NAME)
	@printf '\t$(NAME) deleted\n'
re: fclean all
