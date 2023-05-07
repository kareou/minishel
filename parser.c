/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 12:09:44 by asekkak           #+#    #+#             */
/*   Updated: 2023/05/07 17:57:26 by mkhairou         ###   ########.fr       */
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
	int	k = 0;
	head->flag = 0;
	while (head->str[i])
	{
		if (ft_strncmp(head->str[i], "-n", 2) == 0)
		{
			k = 0;
			if(head->str[i][k] != '-')
				break;
			else
				k++;
			while (head->str[i][k] && head->str[i][k] == 'n' && head->str[i][k + 1] == 'n')
				k++;
			if(!head->str[i][k + 1] && head->str[i + 1])
			{
				head->flag = 1;
				break;
			}
			else
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

char	*check_expanding(t_mshel *shel,char *str)
{
	int		i;
	int		dollar_index;
	char	**tmp;
	char	*value;
	char	*placeholder;

	i = 1;
	dollar_index = find_dollar(str);
	if (!str[dollar_index + 1])
		return(str);
	tmp = ft_split(str,'$');
	value = NULL;
	if(dollar_index == 0)
	{
		i = 0;
		while (tmp[i])
		{
			if(ft_isdigit(tmp[i][0]) || (!ft_isalnum(tmp[i][0]) && (tmp[i][0] != '?' && tmp[i][0] != '_')) )
				value = ft_substr(tmp[i], 1, ft_strlen(tmp[i]));
			else if(tmp[i][0] == '?')
			{
				placeholder = ft_strdup(ft_itoa(shel->exit_status));
				if(value)
					value = ft_strjoin(value, placeholder);
				else
					value = ft_strdup(placeholder);
				if (tmp[i][1])
					value = ft_strjoin(value, tmp[i] + 1);
			}
			else
			{
				int	k = 0;
				while (tmp[i][k] &&( ft_isalnum(tmp[i][k]) || tmp[i][k] == '_'))
					k++;
				placeholder = ft_getenv(shel,ft_substr(tmp[i], 0, k));
				if(placeholder)
				{
					if(value)
						value = ft_strjoin(value, placeholder);
					else
						value = ft_strdup(placeholder);
				}
				else
					value = NULL;
				if(tmp[i][k])
						value = ft_strjoin(value, ft_substr(tmp[i], k, ft_strlen(tmp[i])));
			}
			i++;
		}
	}
	else if (dollar_index > 0 && tmp)
	{
		value = ft_strdup(tmp[0]);
		while (tmp[i])
		{
			if(ft_isdigit(tmp[i][0]))
				value = ft_strjoin(value,tmp[i] + 1);
			if(tmp[i][0] == '?')
			{
				value = ft_strjoin(value, ft_itoa(shel->exit_status));
				if (tmp[i][1])
					value = ft_strjoin(value, tmp[i] + 1);
			}
			else
			{
				int k = 0;
				while (tmp[i][k] && (ft_isalnum(tmp[i][k]) || tmp[i][k] == '_'))
					k++;

				value = ft_strjoin(value, ft_getenv(shel, ft_substr(tmp[i], 0, k)));
				if (tmp[i][k])
					value = ft_strjoin(value, ft_substr(tmp[i], k, ft_strlen(tmp[i])));
			}
			i++;
		}
	}
	return(value);
}

void parser(t_lexer *lst, t_mshel *shel, char *input)
{

	(void)input;
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
		head = head->next;
		i++;
	}
	transfer_to_array(lst, i, shel);
}
