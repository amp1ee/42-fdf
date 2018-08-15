NAME		= fdf
CC			= gcc
SRCDIR		= ./src/
OBJDIR		= ./obj/
HDIR		= ./includes/
LIBDIR		= ./libft/
MLXDIR		= ./minilibx
SRC			= $(addprefix $(SRCDIR), \
				main.c		\
				controls.c	\
				read.c		\
				draw.c		\
				color.c		\
				util.c		\
				readutils.c)
OBJ			= $(SRC:$(SRCDIR)%.c=$(OBJDIR)%.o)
FLAGS		= -Wall -Wextra -Werror -O3
LIBFLAGS	= -lft -L$(LIBDIR) -lmlx
MATH		= -lm

OS_NAME		= $(shell uname -s)

ifeq ($(OS_NAME), Linux)
  LIBFLAGS	+= -lXext -lX11
else
  LIBFLAGS	+= -framework OpenGL -framework AppKit
endif

LIBFT		= $(LIBDIR)libft.a
LIBMLX		= $(MLXDIR)/libmlx.a
INCL		= $(wildcard $(HDIR)*.h)

.PHONY: all debug clean fclean re

all: $(NAME)

$(OBJDIR)%.o: $(SRCDIR)%.c $(INCL)
	@mkdir -p $(OBJDIR)
	@printf '\tCompiling $<\n'
	@gcc $(FLAGS) -c $< -o $@ -I$(HDIR) -I$(MLXDIR)
$(NAME): $(OBJ) $(LIBMLX) $(LIBFT)
	@printf '\tLinking\n'
	@gcc $(OBJ) -o $(NAME) $(LIBFLAGS) $(MATH) -L$(MLXDIR) -F$(MLXDIR)
	@echo 'Done'
$(LIBFT):
	@printf '\tAssembling libft.a\n'
	@make -sC $(LIBDIR)
$(LIBMLX):
	@printf '\tBuilding libmlx.a\n'
	@make -sC $(MLXDIR)
debug: $(SRC) $(FDF_H)
	@gcc -ggdb3 $(FLAGS) -c $(SRC) -I$(HDIR)
	@gcc $(OBJ) -o $(NAME) $(MLXFLAGS) $(MATH) $(LIBFLAGS)

clean:
	@rm -rf $(OBJDIR)
	@printf '\tObject files deleted\n'
fclean: clean
	@make -sC $(LIBDIR) 'fclean'
	@rm -f $(NAME)
	@printf '\t$(NAME) deleted\n'
re: fclean all
