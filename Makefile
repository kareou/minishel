NAME = minishell
Header = minishell.h

FILE = minishell.c norminnette_parsing.c redirect.c utils.c ft_export.c heredoc.c ft_getenv.c parssing_utils.c  parser.c copy_to_array.c commands_2.c commands.c pipe.c run_commands.c
CC = gcc
file = ./libft/libft.a
CFLAGS =  -Wall -Wextra -Werror -fsanitize=address -g


HEAD = -lreadline #-L/goinfre/asekkak/homebrew/opt/readline/lib
INCLUDE= #-I/goinfre/asekkak/homebrew/opt/readline/include


$(RM) = rm -rf

OBG = $(FILE:.c=.o)

all: $(NAME)

$(file):
	make -C libft


%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@


$(NAME): $(OBG) $(file)
	$(CC) $(CFLAGS) $(OBG) $(HEAD) $(file) -o $(NAME)

clean:
	$(RM) $(OBG) ./libft/*.o

fclean: clean
	$(RM) $(NAME)  ./libft/*.a

re: fclean all



.PHONY: bonus all re fclean clean  libft
