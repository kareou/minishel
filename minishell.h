/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 11:29:14 by asekkak           #+#    #+#             */
/*   Updated: 2023/05/07 21:48:00 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <signal.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include "./libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_heredoc
{
	int		heredoc_number;
	char	*cmd;
	char	**delemiter;
} t_heredoc;

typedef	struct s_redr
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
} t_redr;

typedef	struct s_cmd
{
	char	*cmd;
	int		flags;
	char	**args;
	t_redr	redirect;
	int		error;
	char	*error_file;
} t_cmd;

typedef struct s_mshel
{
	t_cmd	**cmd;
	char	**env;
	char	**x_env;
	int		pipes;
	int		cmd_number;
	int		exit_status;
	int		*exapnd_herdoc;
	int		*status;
} t_mshel;

//************* lexer **************

typedef struct s_table
{
 	char *name_file;
	char *token_redire;
 	int redire;
} t_table;

typedef struct s_env
{
 	char *dollar;
	int i;
} t_env;

typedef struct s_lexer
{
	char **str;
	struct s_lexer *next;
	char *cmd;
	int flag;
	int pipe;
	t_table *table;
	t_env *env;
} t_lexer;
//*****************************


//* *********parser****************
void	parser(t_lexer *lst, t_mshel *shel, char *input);
//********************************
t_lexer	*add_node(char **content, int start, int end);
void	ft_add_back(t_lexer **lst, t_lexer *new);
void	ft_sringcpy(char **str, char *content);
void	function();
void	transfer_to_array(t_lexer *lexer, int size_arrays, t_mshel *shel);

//***********piping***********
void	close_all_pipes(int (*pipe)[2], int cmd_numbers);
void	pipe_and_start(t_mshel *mshel);
int 	redirect_to_pipe(t_mshel *shel , int (*pipe)[2], int i, int red_status, int status);
//****************************

//*********execution**********
void	execute_cmd(t_mshel *shel, int (*pipe)[2], int cmd_index, int status);
void	ech_o(t_mshel *shel, int i);
void    execute_shell(char **cmd, t_mshel *shel);
void	p_w_d(void);
int		c_d(t_mshel *shel, char *a);
void	print_env(t_mshel *shel, int stat);
char	*ft_getenv(t_mshel *mshel, char *a);
int		ft_export(t_mshel *shel, int cmd_index);
void	dup_env(t_mshel *shel, char **env);
void	ft_unset(t_mshel *shel, int cmd_index);
void	run_cmd(t_mshel *shel , int cmd_index, char *cmd);
//****************************

//*******redirection**********
int		redirect_input(t_mshel *mshel, int cmd_index, int status);
int		redirect_output(t_mshel *mshel, int cmd_index);
int		check_redirect_place(int in, int out);
void	ft_heredoc(int cmd_index, t_mshel *shel);
//****************************


//********utils***************
void	print_errors(char *a);
//****************************


char	*check_expanding(t_mshel *shel, char *str);

char	**better_parsing(char *a, t_mshel *shel);


long	checking_overwrite(t_mshel *shel,int cmd_index);

void	error_to_print(int error, char *file);


char	*parsse_redirection(char *input, int *s);
char *substr(char const *s, unsigned int start, size_t len);

#endif
