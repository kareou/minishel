/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 11:28:46 by asekkak           #+#    #+#             */
/*   Updated: 2023/05/11 12:08:41 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	check_trim_permission(char	*a)
// {
// 	int	i = 0;

// 	while (a[i])
// 	{
// 		if(a[i] == '=' && a[i + 1] == ' ')
// 			return(0);
// 		else if ()

// 		i++;
// 	}

// }

void ft_sringcpy(char **str, char *content)
{
	int i = 0;

	while (content[i])
	{
		(*str)[i] = content[i];
		i++;
	}

	(*str)[i] = '\0';
}

t_lexer *add_node(char **content, int start, int end)
{
	t_lexer *lexer = malloc(sizeof(t_lexer));
	lexer->str = malloc(sizeof(char *) * (end - start + 1));
	if (!lexer)
		return (0);

	for (int i = start; i < end; i++)
	{
		lexer->str[i - start] = malloc(sizeof(char) * (strlen(content[i]) + 1));
		ft_sringcpy(&(lexer->str[i - start]), content[i]);
	}

	lexer->str[end - start] = NULL;
	lexer->next = NULL;
	return (lexer);
}

void ft_add_back(t_lexer **lst, t_lexer *new)
{
	t_lexer *lastnode;

	lastnode = *lst;
	if (*lst)
	{
		while (lastnode->next)
			lastnode = lastnode->next;
		lastnode->next = new;
	}
	else
		*lst = new;
}

void lexer(char *input, t_mshel *shel)
{
	int i;
	int j;

	i = 0;
	j = 0;
	t_lexer *newNode;
	t_lexer *lst = NULL;
	char **string = calloc(1024, 1024);
	string = better_parsing(input, shel);
	while (string[i])
	{
		if (ft_strncmp(string[i], "|", 2) == 0 && shel->status[i] == 0)
		{
			newNode = add_node(string, j, i + 1);
			ft_add_back(&lst, newNode);
			j = i + 1;
		}
		i++;
	}
	if (i - j >= 1)
	{
		newNode = add_node(string, j, i);
		ft_add_back(&lst, newNode);
	}
	parser(lst, shel, input);
}

void sigint_handler_c(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	// rl_replace_line("", 0);
	rl_redisplay();
}

void sigint_handler_quit(int sig)
{
	(void)sig;
}

int find_file(int i, char *a)
{
	int state = 0;
	while (i)
	{
		if(ft_isalnum(a[i]) || a[i] == '\'' || a[i] == '\"')
			state = 1;
		i--;
	}
	return(state);
}

int check_syntax(char *a)
{
	int i = 0;
	int quotes = 0;
	int prev_symbol = 0;

	while (a[i])
	{
		if (a[i] == ' ' && !a[i + 1] && !quotes && prev_symbol)
		{
			print_errors("minishell: syntax error ");
			return 0;
		}
		if (a[i] == '"' || a[i] == '\'')
		{
			if (!quotes)
				quotes = a[i];
			else if (quotes == a[i])
				quotes = 0;
		}
		else if (!quotes && (a[i] == '<' || a[i] == '>' || a[i] == '|'))
		{
			if (i == 0 || a[i + 1] == '\0' || a[i - 1] == '|')
			{
				if (i == 0 && a[i] != '<' && a[i] != '>')
				{
					print_errors("minishell: syntax error");
					return 0;
				}
				else if (!a[i + 1])
				{
					print_errors("minishell: syntax error");
					return 0;
				}
			}
			else if (a[i] == '<' && a[i + 1] == '>')
			{
				print_errors("minishell: syntax error");
				return 0;
			}
			else if (a[i] == '>' && a[i + 1] == '<')
			{
				print_errors("minishell: syntax error");
				return 0;
			}
			else if (a[i] == '|' && !find_file(i, a))
			{
				print_errors("minishell: syntax error");
				return 0;
			}
			else
			{
				prev_symbol = 1;
			}
		}
		else
		{
			prev_symbol = 0;
		}
		i++;
	}

	if (quotes != 0)
	{
		print_errors("minishell: unexpected EOF while looking for matching");
		return 0;
	}

	return 1;
}

void minishell(char **env)
{
	char *input;
	t_mshel *shel;

	signal(SIGQUIT, sigint_handler_quit);
	signal(SIGINT, sigint_handler_c);
	shel = malloc(sizeof(t_mshel));
	dup_env(shel, env);
	shel->exit_status = 0;
	shel->exapnd_herdoc = malloc(sizeof(int) * 10);
	shel->status = malloc(sizeof(int) * 10000);
	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL)
		{
			//  printf("exit\n");
			exit(shel->exit_status);
		}
		// if (ft_strncmp(input, "exit",5) == 0) {
		//     printf("Exiting...\n");
		//     break;
		// }
		add_history(input);
		if (check_syntax(ft_strtrim(input, " ")))
			lexer(input, shel);
		else
			shel->exit_status = 2;
		free(input);
	}
}

int main(int ac, char **av, char **env)
{

	(void)ac;
	(void)av;
	minishell(env);
}
