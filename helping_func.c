/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helping_func.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 12:35:37 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/24 12:41:25 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_heredoc(t_mshel *shel, int status, int cmd_index)
{
	int	i;
	int	exited;

	if (status == 0)
	{
		i = fork();
		if (i == 0)
			ft_heredoc(cmd_index, shel);
		else
		{
			shel->id = i;
			waitpid(i, &exited, 0);
		}
		shel->exit_status = WEXITSTATUS(exited);
	}
	else
		ft_heredoc(cmd_index, shel);
}

void	copy_pipe(char **b, int *j)
{
	(*b)[(*j)++] = ' ';
	(*b)[(*j)++] = '|';
	(*b)[(*j)++] = ' ';
}

char	*fix_pipe(char *a)
{
	int		i;
	int		j;
	int		quote;
	char	*b;

	i = -1;
	j = 0;
	quote = 0;
	b = malloc(sizeof(char) * (ft_strlen(a) * 2));
	while (a[++i])
	{
		if (a[i] == '"' || a[i] == '\'')
		{
			if (quote == 0)
				quote = a[i];
			else
				quote = 0;
		}
		if (a[i] == '|' && quote == 0)
			copy_pipe(&b, &j);
		else
			b[j++] = a[i];
	}
	b[j] = '\0';
	return (b);
}

void	set_null(t_mshel *shel, t_indexs *index, t_lexer *lexer, int action)
{
	if (action == 0)
	{
		index->i = 0;
		index->r = 0;
		index->o = 0;
	}
	else
	{
		shel->cmd[index->k]->args[index->i] = NULL;
		if (lexer->table->redire > 0)
		{
			shel->cmd[index->k]->redirect.input[index->r] = NULL;
			shel->cmd[index->k]->redirect.in_file[index->r] = NULL;
			shel->cmd[index->k]->redirect.output[index->o] = NULL;
			shel->cmd[index->k]->redirect.out_file[index->o] = NULL;
		}
	}
}
