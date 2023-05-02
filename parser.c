/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 12:09:44 by asekkak           #+#    #+#             */
/*   Updated: 2023/05/02 16:41:18 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *all_cmd(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 6) == 0)
		return ("echo");
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		return ("cd");
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		return ("pwd");
	else if (ft_strncmp(cmd, "export", 7) == 0)
		return ("export");
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		return ("unset");
	else if (ft_strncmp(cmd, "env", 4) == 0)
		return ("env");
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		return ("exit");
	return (0);
}

char *all_redir(char *redir)
{
	if (ft_strncmp(redir, ">", 6) == 0)
		return ("echo");
	else if (ft_strncmp(redir, ">>", 3) == 0)
		return ("cd");
	else if (ft_strncmp(redir, "<", 4) == 0)
		return ("pwd");
	else if (ft_strncmp(redir, "<<", 7) == 0)
		return ("export");

	return (0);
}

int check_cmd(t_lexer *head)
{
	int i = 0;
	while (head->str[i])
	{
		if (all_cmd(head->str[i]))
		{
			head->cmd = all_cmd(head->str[i]);
			return (0);
		}
		i++;
	}
	return (1);
}

int check_flag(t_lexer *head)
{
	int i = 0;
	head->flag = 0;
	while (head->str[i])
	{
		if (ft_strncmp(head->str[i], "-n", 3) == 0)
		{
			head->flag = 1;
			return (0);
		}
		i++;
	}
	return (1);
}
char *choose_red(char *red)
{
	if (ft_strncmp(red, ">", 2) == 0)
		return (">");
	else if (ft_strncmp(red, "<", 2) == 0)
		return ("<");
	else if (ft_strncmp(red, ">>", 3) == 0)
		return (">>");
	else if (ft_strncmp(red, "<<", 3) == 0)
		return ("<<");
	return (0);
}

int check_pipe(t_lexer *head)
{
	int i = 0;
	head->pipe = 0;
	while (head->str[i])
	{
		if (ft_strncmp(head->str[i], "|", 2) == 0)
		{
			head->pipe = 1;
			return (0);
		}
		i++;
	}
	return (1);
}

int check_dollar(t_lexer *head)
{
	int i = 0;
	int j = 0;
	int count = 0;
	char *str;

	while (head->str[i])
	{
		if ((str = ft_strchr(head->str[i], '$')))
			count++;
		i++;
	}
	if (!(head->env = malloc(sizeof(t_env) * (count + 1))))
		return (0);
	i = 0;
	j = 0;
	while (head->str[i])
	{
		if ((str = ft_strchr(head->str[i], '$')))
		{
			head->env[j].dollar = str;
			head->env->i = j;
			j++;
		}
		i++;
	}
	return (0);
}

int check_redirection(t_lexer *lexer)
{
	int i = 0;
	int j = 0;
	int count = 0;

	while (lexer->str[i])
	{
		if (choose_red(lexer->str[i]))
			count++;
		i++;
	}
	i = 0;

	lexer->table = malloc(sizeof(t_table) * (count + 1));
	if (!lexer->table)
		return (0);
	while (lexer->str[i])
	{
		lexer->table[j].redire = 0;
		if (choose_red(lexer->str[i]))
		{
			int match = ft_strncmp(lexer->str[i], choose_red(lexer->str[i]), strlen(choose_red(lexer->str[i]) + 1));
			if (match == 0)
			{
				// printf("SET table at %d\n", j);
				lexer->table[j].redire = 1;
				lexer->table[j].name_file = lexer->str[i + 1];
				lexer->table[j].token_redire = choose_red(lexer->str[i]);
				j++;
			}
		}
		i++;
	}
	return (1);
}

int	find_dollar(char *a)
{
	int	i;

	i = 0;
	while (a[i])
	{
		if(a[i] == '$')
			return(i);
		i++;
	}
	return(-1);
}

void	check_expanding(t_lexer *lst, t_mshel *shel)
{
	int		i;
	int		dollar_index;
	char	**tmp;

	i = 0;
	while (lst->str[i])
	{
		if(ft_strchr(lst->str[i],'$'))
		{
			dollar_index = find_dollar(lst->str[i]);
			tmp = ft_split(lst->str[i],'$');
			if(dollar_index == 0)
			{
				free(lst->str[i]);
				if(ft_isdigit(tmp[0][0]))
					lst->str[i] = ft_strdup(tmp[0] + 1);
				else
					lst->str[i] = ft_strdup(ft_getenv(shel, tmp[0]));
			}
			else
			{
				free(lst->str[i]);
				lst->str[i] = ft_strdup(tmp[0]);
				if(ft_isdigit(tmp[1][0]))
					lst->str[i] = ft_strjoin(lst->str[i],tmp[1] + 1);
				else
					lst->str[i] = ft_strjoin(lst->str[i], ft_getenv(shel, tmp[1]));
			}
		}
		i++;
	}
}

void parser(t_lexer *lst, t_mshel *shel)
{

	t_lexer *head = lst;
	int		i = 0;

	while (head)
	{

		if (check_cmd(head))
			head->cmd = "empty";
		check_flag(head);
		check_pipe(head);
		check_dollar(head);
		check_redirection(head);
		check_expanding(head, shel);
		head = head->next;
		i++;
	}
	transfer_to_array(lst, i, shel);
	// t_lexer *test = lst;
	// int i = 0;
	// int j = 0;
	// int t = 0;
	// while (test)
	// {
	// 	printf("/****************************/\n");
	// 	j = 0;
	// 	i = 0;
	// 	t = 0;
	// 	printf("	String : [");
	// 	while (test->str[i])
	// 		printf(" {%s} ", test->str[i++]);
	// 	printf("]\n");
	// 	printf("	Commande: {%s}, \n", test->cmd);
	// 	printf("	Flag: %d\n", test->flag);
	// 	printf("	Pipe: %d\n", test->pipe);
	// 	while (t <= test->env->i)
	// 		printf("	ENV: %s\n", test->env[t++].dollar);
	// 	while (test->table[j].redire == 1)
	// 	{
	// 		printf("	Token: %d\n", test->table[j].redire);
	// 		printf("	Redirection token: %s\n", test->table[j].token_redire);
	// 		printf("	Name File Redirection : %s\n", test->table[j].name_file);
	// 		j++;
	// 	}
	// 	printf("/******* NEXT NODE **********/\n");
	// 	test = test->next;
	// }
}
