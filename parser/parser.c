/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 12:09:44 by asekkak           #+#    #+#             */
/*   Updated: 2023/05/22 12:24:42 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

int	check_pipe(t_lexer *head)
{
	int	i;

	i = 0;
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

void	separat_check_dollar(int i, int j, t_lexer *head)
{
	while (head->str[i])
	{
		if ((ft_strchr(head->str[i], '$')))
		{
			head->env[j].dollar = \
			ft_strchr(head->str[i], '$');
			head->env->i = j;
			j++;
		}
		i++;
	}
}

int	check_dollar(t_lexer *head)
{
	int		i;
	int		j;
	int		count;

	i = 0;
	j = 0;
	count = 0;
	while (head->str[i])
	{
		if ((ft_strchr(head->str[i], '$')))
			count++;
		i++;
	}
	head->env = malloc(sizeof(t_env) * (count + 1));
	if (!head->env)
		return (0);
	i = 0;
	separat_check_dollar(i, j, head);
	return (0);
}

void	parser(t_lexer *lst, t_mshel *shel, char *input)
{
	t_lexer	*head;
	int		i;

	(void)input;
	head = lst;
	i = 0;
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
