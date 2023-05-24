/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 21:25:10 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/24 11:42:38 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_n_close_p(int (*pipes)[2], int cs, int p_number)
{
	int	i;

	i = 0;
	if (cs == 0)
	{
		while (i < p_number)
		{
			pipe(pipes[i]);
			i++;
		}
	}
	else
	{
		while (i < p_number)
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
			i++;
		}
	}
}

void	printf_in_pipe(char *a, int fd, int action)
{
	int	i;

	i = 0;
	if (!a)
	{
		write(fd, "\n", 1);
		return ;
	}
	while (a[i])
	{
		write(fd, &a[i], 1);
		i++;
	}
	write(fd, "\n", 1);
	if (action)
	{
		if (a)
			free(a);
	}
}

int	check_input(t_mshel *shel, int cmd_index, int *i, char **a)
{
	char	*holder;

	holder = NULL;
	if (ft_strchr((*a), '$'))
		holder = check_expanding(shel, (*a));
	if (((*a)[0] == '\'' && \
	shel->cmd[cmd_index]->redirect.heredoc.delemiter[(*i)][0] == '\'') \
	|| ((*a)[0] == '"' && \
	shel->cmd[cmd_index]->redirect.heredoc.delemiter[(*i)][0] == '"'))
		holder = remove_quotes((*a), (*a)[0]);
	if (!ft_strcmp((*a), \
	shel->cmd[cmd_index]->redirect.heredoc.delemiter[(*i)]) \
	|| (holder && !ft_strcmp(holder, \
	shel->cmd[cmd_index]->redirect.heredoc.delemiter[(*i)])))
	{
		free(*a);
		(*a) = NULL;
		(*i)++;
	}
	free(holder);
	if ((*i) >= shel->cmd[cmd_index]->redirect.heredoc.heredoc_number)
		return (0);
	return (1);
}

void	read_line(t_mshel *shel, int cmd_index, int *i, int (*pipes)[2])
{
	char	*a;

	while (1)
	{
		a = readline("heredoc> ");
		if (a == NULL)
		{
			(*i)++;
			break ;
		}
		if (!check_input(shel, cmd_index, i, &a))
		{
			free(a);
			break ;
		}
		if (a)
		{
			if (ft_strchr(a, '$') && shel->exapnd_herdoc[(*i)] == 1)
				printf_in_pipe(check_expanding(shel, a), pipes[(*i)][1], 1);
			else
				printf_in_pipe(a, pipes[(*i)][1], 0);
		}
		free(a);
	}
}
