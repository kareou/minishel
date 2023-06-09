/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 11:29:14 by asekkak           #+#    #+#             */
/*   Updated: 2023/05/25 22:16:29 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>

typedef struct s_heredoc
{
	int				heredoc_number;
	char			*cmd;
	char			**delemiter;
}					t_heredoc;

typedef struct s_redr
{
	char			**input;
	char			**output;
	char			**in_file;
	char			**out_file;
	int				*input_expanded;
	int				*output_expanded;
	int				old_output;
	int				old_input;
	int				in;
	int				out;
	t_heredoc		heredoc;
	int				ambugius;
}					t_redr;

typedef struct s_cmd
{
	char			*cmd;
	int				flags;
	char			**args;
	t_redr			redirect;
	int				error;
	char			*error_file;
	int				red;
}					t_cmd;

typedef struct s_mshel
{
	t_cmd			**cmd;
	char			**env;
	char			**x_env;
	int				pipes;
	int				cmd_number;
	int				exit_status;
	int				*exapnd_herdoc;
	int				*status;
	int				stat;
	int				herdoc_number;
	int				break_heardoc;
	char			*store_input;
	int				id;
}					t_mshel;

//************* lexer **************

typedef struct s_indexs
{
	int				i;
	int				j;
	int				o;
	int				r;
	int				k;
	int				du;
	int				duin;
	char			**new;
	char			*a;
}					t_indexs;

typedef struct s_table
{
	char			*name_file;
	char			*token_redire;
	int				redire;
}					t_table;

typedef struct s_env
{
	char			*dollar;
	int				i;
}					t_env;

typedef struct s_lexer
{
	char			**str;
	struct s_lexer	*next;
	char			*cmd;
	int				flag;
	int				pipe;
	t_table			*table;
	t_env			*env;
}					t_lexer;
//*****************************

//* *********parser****************
void				parser(t_lexer *lst, t_mshel *shel, char *input);
//********************************
t_lexer				*add_node(char **content, int start, int end);
void				ft_add_back(t_lexer **lst, t_lexer *new);
void				ft_sringcpy(char **str, char *content);
void				function(void);
void				transfer_to_array(t_lexer *lexer, int size_arrays,
						t_mshel *shel);

//***********piping***********
void				close_all_pipes(int (*pipe)[2], int cmd_numbers);
void				pipe_and_start(t_mshel *mshel);
int					redirect_to_pipe(t_mshel *shel, int (*pipe)[2], int i,
						int red_status, int status);
//****************************

//*********execution**********
void				execute_cmd(t_mshel *shel, int (*pipe)[2], int cmd_index,
						int status);
void				ech_o(t_mshel *shel, int i);
void				execute_shell(char **cmd, t_mshel *shel);
void				p_w_d(void);
int					c_d(t_mshel *shel, char *a);
void				print_env(t_mshel *shel, int stat);
char				*ft_getenv(t_mshel *mshel, char *a);
int					ft_export(t_mshel *shel, int cmd_index, int i);
void				dup_env(t_mshel *shel, char **env);
int					ft_unset(t_mshel *shel, int cmd_index);
void				run_cmd(t_mshel *shel, int cmd_index, char *cmd);
//****************************

//*******redirection**********
int					redirect_input(t_mshel *mshel, int cmd_index, int status);
int					redirect_output(t_mshel *mshel, int cmd_index);
int					check_redirect_place(int in, int out);
void				ft_heredoc(int cmd_index, t_mshel *shel);
//****************************

//********utils***************
void				print_errors(char *a);
//****************************

char				*check_expanding(t_mshel *shel, char *str);
char				**better_parsing(char *a, t_mshel *shel, t_indexs *index);
long				checking_overwrite(t_mshel *shel, int cmd_index);
void				error_to_print(int error, char *file);
char				*parsse_redirection(char *input, int *s);
char				*substr(char const *s, unsigned int start, size_t len);

////testing
void				hendel_no_quotes(t_mshel *shel, t_indexs *index,
						char *tmp);
void				handel_sing_quote(t_mshel *shel, char *a, t_indexs *index);
int					check_space_place(char *a);
char				*ft_strtr(char *s1, char *set);
int					theres_is_red(char *a);
void				handel_double_quotes(t_mshel *shel, char *a,
						t_indexs *index, int cp);
void				handel_no_quotes_expand(char *tempo,
						t_indexs *index, char a, t_mshel *shel);
void				hendel_no_quotes_spand_j(t_mshel *shel, char *tempo,
						t_indexs *index, char a);
void				hande_no_quoet_expand_n(t_mshel *shel,
						t_indexs *index, char *a, int check_point);
int					array_lenth(char **a);
void				open_n_close_p(int (*pipes)[2], int cs, int p_number);
void				free_lexer(t_lexer *lexer);
void				free_all(t_mshel *shel, int size);
int					exit_function(char **a, t_mshel *shel);
int					add_x_env(t_mshel *shel, char *variable);
int					add_env(t_mshel *shel, char *variable, int i);

// check_syntax.c
int					check_syntax(char *a, int i, int quotes);

// lexer part
void				lexer(char *input, t_mshel *shel);

// node_lk_list
t_lexer				*add_node(char **content, int start, int end);
void				ft_add_back(t_lexer **lst, t_lexer *new);

// file parser/parser.c
int					check_redirection(t_lexer *lexer);
int					check_flag(t_lexer *head);
int					check_cmd(t_lexer *head);
int					check_pipe(t_lexer *head);
char				*choose_red(char *red);
char				*all_cmd(char *cmd);
char				*all_redir(char *redir);
int					find_dollar(char *a);
char				*check_expanding(t_mshel *shel, char *str);
int					ft_strcmp(char *a, char *b);
void				transfer(t_lexer *lexer, t_mshel *shel, int j, int k);
void				transfer_cmd(t_lexer *lexer, t_mshel *shel, int i);
void				no_quot_part(char *a, t_mshel *shel, t_indexs *index);
void				read_line(t_mshel *shel, int cmd_index, int *i, \
int (*pipes)[2]);
char				*remove_quotes(char *a, int c);
void				printf_in_pipe(char *a, int fd, int action);
void				split_pipe(char *a, t_mshel *shel, t_indexs *index);
void				free_array(char **a);
int					check_valid(char *a, int declare, \
t_mshel *shel);
void				free_array(char **string);
char				*smart_join(char *a, char *b);
int					reset_redirection(t_mshel *shel, int cmd_index, int status);
void				exiting(t_mshel *shel, int exit_status, int free_all_flag);
int					word_num(char *a, int c);
int					count_array(char **a, char *c, char *d);
void				sigint_handler_c(int sig);
void				run_heredoc(t_mshel *shel, int status, int cmd_index);
char				*fix_pipe(char *a);
void				set_null(t_mshel *shel, t_indexs *index, t_lexer *lexer, \
int action);
void				expande_dollare_s(int i, char **value, char **tmp, \
t_mshel *shel);
char				*fix_path(char *a, int i, int j);

#endif
