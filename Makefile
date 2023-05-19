NAME = minishell
Header = minishell.h

FILE = minishell.c norminnette_parsing.c redirect.c utils.c ft_export.c heredoc.c ft_getenv.c ./parser/parssing_utils.c  ./parser/parser.c copy_to_array.c commands_2.c commands.c pipe.c run_commands.c \
		check_syntax.c ./lexer/lexer_part.c ./lexer/node_lk_list.c ./parser/check_cmd_red_flag.c  ./parser/compare_cmd_red_dollar.c ./parser/check_expanding.c\
		memory_leaks.c parsing_d_quote.c exit_cmd.c ft_strcmp.c pipe_utils.c copy_to_array_utils.c\
		start_copy.c utils_2.c pwd.c heredoc_utils.c
CC =  cc
file = ./libft/libft.a
CFLAGS =  -Wall -Wextra -Werror #-fsanitize=address -g


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
