/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asekkak <asekkak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 12:09:44 by asekkak           #+#    #+#             */
/*   Updated: 2023/05/13 11:05:30 by asekkak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

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


void separat_check_dollar(int i, int j, t_lexer *head, char *str)
{
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
}

int check_dollar(t_lexer *head)
{
	int i;
	int j;
	int count;
	char *str;

	i = 0;
	j = 0;
	count = 0;
	while (head->str[i])
	{
		if ((str = ft_strchr(head->str[i], '$')))
			count++;
		i++;
	}
	if (!(head->env = malloc(sizeof(t_env) * (count + 1))))
		return (0);
	i = 0;
	separat_check_dollar(i, j, head, str);
	return (0);
}

void parser(t_lexer *lst, t_mshel *shel, char *input)
{

	(void)input;
	t_lexer *head = lst;
	int i = 0;

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
