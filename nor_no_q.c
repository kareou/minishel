/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nor_no_q.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 21:50:29 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/20 22:06:38 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	join_no_s(t_mshel *shel, t_indexs *index, char *tempo, char **t)
{
	int	ind;

	if (index->j != 0 && !theres_is_red(index->new[index->j - 1]))
	{
		index->new[index->j++] = ft_strdup(tempo);
		shel->status[shel->stat++] = 1;
	}
	else
	{
		ind = 0;
		if (index->j != 0 && !ft_strcmp(index->new[index->j - 1], "<<"))
			shel->exapnd_herdoc[shel->herdoc_number] = 1;
		if (!t[ind])
			index->new[index->j++] = ft_calloc(1, 1);
		else
		{
			while (t[ind])
			{
				index->new[index->j++] = ft_strtrim(t[ind], " ");
				shel->status[shel->stat++] = 1;
				ind++;
			}
		}
	}
}

void	check_if_heredoc_exp(t_indexs *index, t_mshel *shel)
{
	if (index->j != 0 && !ft_strcmp(index->new[index->j - 1], "<<"))
	{
		shel->exapnd_herdoc[shel->herdoc_number] = 1;
		shel->herdoc_number++;
	}
}

void	joining_w_prev(t_mshel *shel, char *tempo, t_indexs *index, char a)
{
	char	**t;

	check_if_heredoc_exp(index, shel);
	t = ft_split(tempo, ' ');
	if (array_lenth(t) == 1)
	{
		if (a == ' ')
			index->new[index->j++] = ft_strtr(tempo, " ");
		else
		{
			if (!a)
				index->new[index->j++] = ft_strdup(t[0]);
			else
				index->new[index->j++] = ft_strdup(tempo);
		}
		shel->status[shel->stat++] = 1;
	}
	else
		join_no_s(shel, index, tempo, t);
}

void	hendel_no_quotes_spand_j(t_mshel *shel, char *tempo, t_indexs *index,
		char a)
{
	if (tempo && ft_strchr(tempo, ' '))
		joining_w_prev(shel, tempo, index, a);
	else if (tempo)
	{
		check_if_heredoc_exp(index, shel);
		index->new[index->j++] = ft_strdup(tempo);
		shel->status[shel->stat++] = 1;
	}
	else
	{
		check_if_heredoc_exp(index, shel);
		index->new[index->j++] = ft_calloc(1, 1);
		shel->status[shel->stat++] = 1;
	}
}
