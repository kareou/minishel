/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_to_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 10:46:32 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/24 12:34:21 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	input_count(char **a, int state)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (a[i])
	{
		if ((!ft_strcmp(a[i], "<") || !ft_strcmp(a[i], "<<")) && state == 1)
			j++;
		if ((!ft_strcmp(a[i], ">") || !ft_strcmp(a[i], ">")) && state == 0)
			j++;
		i++;
	}
	return (0);
}

int	find_cmd(char **a, t_mshel *shel)
{
	int	i;

	i = 0;
	if (!a[0])
		return (-2);
	while (a[i])
	{
		if ((a[i] && (!ft_strcmp(a[i], ">") || !ft_strcmp(a[i], "<") \
		|| !ft_strcmp(a[i], ">>") || \
		!ft_strcmp(a[i], "<<"))) && shel->status[i] == 0)
		{
			if (!a[i + 1])
				i++;
			else
				i += 2;
		}
		else if (a[i] && a[i][0] && !ft_strcmp(a[i], "|") && i != 0)
			i++;
		else if (a[i])
			return (i);
	}
	return (-1);
}

void	alloc_redirect(t_mshel *shel, int i, char **a)
{
	int	j;

	(void)a;
	shel->cmd[i]->red = 1;
	shel->cmd[i]->redirect.output = malloc(sizeof(char *) * \
	(1 + count_array(a, ">", ">>")));
	shel->cmd[i]->redirect.out_file = malloc(sizeof(char *) * \
	(1 + count_array(a, ">", ">>")));
	shel->cmd[i]->redirect.input_expanded = malloc(sizeof(char *) * \
	(1 + count_array(a, "<", "<<")));
	shel->cmd[i]->redirect.output_expanded = malloc(sizeof(char *)
			* (1 + count_array(a, ">", ">>")));
	shel->cmd[i]->redirect.input = malloc(sizeof(char *) * \
	(1 + count_array(a, "<", "<<")));
	shel->cmd[i]->redirect.in_file = malloc(sizeof(char *) * \
	(1 + count_array(a, "<", "<<")));
	shel->cmd[i]->redirect.heredoc.cmd = ft_calloc(1, 1);
	shel->cmd[i]->redirect.heredoc.delemiter = malloc(sizeof(char *)
			* (1 + count_array(a, "<", "<<")));
	j = 0;
	while (j < count_array(a, "<", "<<"))
		shel->cmd[i]->redirect.heredoc.delemiter[j++] = NULL;
}

void	transfer_herdoc_args(t_mshel *shel, int i, \
int cmd_position, t_lexer *head)
{
	char	*tmp;
	char	*tmp2;

	if (shel->cmd[i]->cmd)
	{
		tmp2 = shel->cmd[i]->redirect.heredoc.cmd;
		shel->cmd[i]->redirect.heredoc.cmd = \
		ft_strdup(shel->cmd[i]->cmd);
		free(tmp2);
	}
	if (head->str[cmd_position + 1] && \
	ft_strcmp(head->str[cmd_position + 1], "<<"))
	{
		tmp = ft_strjoin(" ", head->str[cmd_position + 1]);
		shel->cmd[i]->redirect.heredoc.cmd = smart_join(\
		shel->cmd[i]->redirect.heredoc.cmd, tmp);
		free(tmp);
	}
}

void	transfer_cmd(t_lexer *lexer, t_mshel *shel, int i)
{
	int		j;
	int		cmd_position;
	t_lexer	*head;

	head = lexer;
	while (head)
	{
		j = 0;
		cmd_position = find_cmd(head->str, shel);
		if (cmd_position == -1)
			shel->cmd[i]->cmd = NULL;
		else
			shel->cmd[i]->cmd = ft_strdup(head->str[cmd_position]);
		if (head->flag != 0)
			shel->cmd[i]->flags = 1;
		if (head->table->redire > 0)
			alloc_redirect(shel, i, head->str);
		transfer(head, shel, cmd_position, i);
		if (shel->cmd[i]->redirect.heredoc.heredoc_number > 0)
			transfer_herdoc_args(shel, i, cmd_position, head);
		i++;
		head = head->next;
	}
	shel->cmd_number = i;
}
