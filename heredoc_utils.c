/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 21:25:10 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/19 21:29:21 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_line(t_mshel *shel, int cmd_index, int *i, int (*pipes)[2])
{
	char	*a;
	char	*holder;

	holder = NULL;
	while (1)
	{
		a = readline("heredoc> ");
		if (a == NULL)
		{
			(*i)++;
			break ;
		}
		if (ft_strchr(a, '$'))
			holder = check_expanding(shel, a);
		if ((a[0] == '\'' && \
		shel->cmd[cmd_index]->redirect.heredoc.delemiter[(*i)][0] == '\'') \
		|| (a[0] == '"' && \
		shel->cmd[cmd_index]->redirect.heredoc.delemiter[(*i)][0] == '"'))
			holder = remove_quotes(a, a[0]);
		if (!ft_strcmp(a, \
		shel->cmd[cmd_index]->redirect.heredoc.delemiter[(*i)]) \
		|| (holder && !ft_strcmp(holder, \
		shel->cmd[cmd_index]->redirect.heredoc.delemiter[(*i)])))
		{
			a = NULL;
			(*i)++;
		}
		if ((*i) >= shel->cmd[cmd_index]->redirect.heredoc.heredoc_number)
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
