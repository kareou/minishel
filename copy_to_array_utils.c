/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_to_array_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 16:00:15 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/20 20:06:54 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	transfer_input(t_lexer *lexer, t_mshel *shel, \
int *start, t_indexs *dx)
{
	shel->cmd[dx->k]->redirect.in = 1;
	shel->cmd[dx->k]->redirect.input[dx->r] = ft_strdup(lexer->str[(*start)]);
	if (lexer->str[(*start) + 1] && !lexer->str[(*start) + 1][0])
	{
		shel->cmd[dx->k]->redirect.in_file[dx->r] = NULL;
		shel->cmd[dx->k]->redirect.input_expanded[dx->r] = shel->status[(*start)
			+ 1];
		(*start) += 1;
	}
	else if (lexer->str[(*start) + 1])
	{
		shel->cmd[dx->k]->redirect.in_file[dx->r] = \
		ft_strdup(lexer->str[(*start) + 1]);
		shel->cmd[dx->k]->redirect.input_expanded[dx->r] = shel->status[(*start)
			+ 1];
		(*start) += 2;
	}
	else
	{
		shel->cmd[dx->k]->redirect.in_file[dx->r] = ft_calloc(1, 1);
		shel->cmd[dx->k]->redirect.input_expanded[dx->r] = shel->status[(*start)
			+ 1];
		(*start)++;
	}
	dx->r++;
}

void	transfer_output(t_lexer *lexer, t_mshel *shel, \
int *start, t_indexs *dx)
{
	shel->cmd[dx->k]->redirect.out = 1;
	shel->cmd[dx->k]->redirect.output[dx->o] = ft_strdup(lexer->str[(*start)]);
	if (lexer->str[(*start) + 1] && !lexer->str[(*start) + 1][0])
	{
		shel->cmd[dx->k]->redirect.out_file[dx->o] = NULL;
		shel->cmd[dx->k]->redirect.output_expanded[dx->o] = \
		shel->status[(*start) + 1];
		(*start) += 1;
	}
	else if (lexer->str[(*start) + 1])
	{
		shel->cmd[dx->k]->redirect.out_file[dx->o] = \
		ft_strdup(lexer->str[(*start) + 1]);
		shel->cmd[dx->k]->redirect.output_expanded[dx->o] = \
		shel->status[(*start) + 1];
		(*start) += 2;
	}
	else
	{
		shel->cmd[dx->k]->redirect.out_file[dx->o] = ft_calloc(1, 1);
		shel->cmd[dx->k]->redirect.output_expanded[dx->o] = \
		shel->status[(*start) + 1];
		(*start)++;
	}
	dx->o++;
}

void	transfer_redirection(t_lexer *lexer, t_mshel *shel, \
int *start, t_indexs *index)
{
	if (!ft_strcmp(lexer->str[(*start)], "<"))
		transfer_input(lexer, shel, start, index);
	if (lexer->str[(*start)] && !ft_strcmp(lexer->str[(*start)], "<<"))
	{
		shel->cmd[index->k]->redirect.heredoc.heredoc_number++;
		(*start)++;
	}
	if (lexer->str[(*start)] && (!ft_strcmp(lexer->str[(*start)], ">")
			|| !ft_strcmp(lexer->str[(*start)], ">>")))
		transfer_output(lexer, shel, start, index);
}

void	transfer_part_2(t_lexer *lexer, t_mshel *shel, \
int *start, t_indexs *index)
{
	if ((!ft_strcmp(lexer->str[(*start)], ">") || !ft_strcmp(\
	lexer->str[(*start)], "<") || !ft_strcmp(lexer->str[(*start)], ">>") || \
	!ft_strcmp(lexer->str[(*start)], "<<")) && shel->status[(*start)] == 0)
		transfer_redirection(lexer, shel, start, index);
	else
	{
		if (lexer->str[(*start)])
		{
			if (ft_strcmp(lexer->str[(*start)], "<<") && (*start) > 0 && \
			shel->cmd[index->k]->redirect.heredoc.heredoc_number > 0
				&& !ft_strcmp(lexer->str[(*start) - 1], "<<"))
			{
				shel->cmd[index->k]->redirect.heredoc.delemiter[shel->cmd \
				[index->k]->redirect.heredoc.heredoc_number - 1] \
				= ft_strdup(lexer->str[(*start)]);
			}
			else
			{
				shel->cmd[index->k]->args[index->i] = \
				ft_strdup(lexer->str[(*start)]);
				index->i++;
			}
			(*start)++;
		}
	}
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

void	transfer(t_lexer *lexer, t_mshel *shel, int j, int k)
{
	t_indexs	index;
	int			start;

	start = 0;
	index.k = k;
	set_null(shel, &index, lexer, 0);
	while (lexer->str[start])
	{
		if (start == j)
		{
			start++;
			if (lexer->flag)
				start++;
		}
		if (lexer->str[start])
		{
			if (ft_strcmp(lexer->str[start], "|") == 0 && start != 0
				&& shel->status[start] == 0)
				start++;
		}
		if (lexer->str[start])
			transfer_part_2(lexer, shel, &start, &index);
	}
	set_null(shel, &index, lexer, 1);
}
