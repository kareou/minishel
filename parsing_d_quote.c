/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_d_quote.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 11:10:30 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/23 16:21:08 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	join_it(char **new, char *tp, t_mshel *shel, int *j)
{
	char	*to_free;
	char	*point;

	if (ft_strchr(new[(*j) - 1], ' ') && check_space_place \
	(new[(*j) - 1]) == 1 && shel->status[shel->stat - 1] == 1)
	{
		to_free = ft_strtr(new[(*j) - 1], " ");
		new[(*j) - 1] = ft_strjoin(to_free, tp);
		free(to_free);
	}
	else
	{
		point = new[(*j) - 1];
		new[(*j) - 1] = ft_strjoin(new[(*j) - 1], tp);
		free(point);
	}
}

void	d_quotes_j(char *tp, char **new, t_mshel *shel, int *j)
{
	if ((*j) != 0)
	{
		if (new[(*j) - 1][0] && tp[0])
			join_it(new, tp, shel, j);
		else
		{
			if (tp[0])
			{
				free(new[(*j) - 1]);
				new[(*j) - 1] = ft_strdup(tp);
			}
			else if (!new[(*j) - 1][0])
			{
				free(new[(*j) - 1]);
				new[(*j) - 1] = NULL;
			}
		}
	}
	else
	{
		if (tp)
			new[(*j)++] = tp;
		else
			new[(*j)++] = ft_calloc(1, 1);
	}
}

void	d_quote_expand(char *a, int checkpoint, t_indexs *idx,
		t_mshel *shel)
{
	char	*exp;
	char	*tet;

	exp = substr(a, checkpoint + 1, idx->i - 1 - checkpoint);
	tet = check_expanding(shel, exp);
	free(exp);
	if (checkpoint != 0 && a[checkpoint - 1] != ' ')
	{
		if (tet)
			idx->new[idx->j - 1] = smart_join(idx->new[idx->j - 1], tet);
	}
	else
	{
		if (tet)
			idx->new[idx->j++] = strdup(tet);
		else
			idx->new[idx->j++] = NULL;
	}
	if (tet)
		free(tet);
	shel->status[shel->stat++] = 0;
}

void	d_quote_copy(t_mshel *shel, t_indexs *index, int checkpoint, char *a)
{
	char	*to_be_freed;

	to_be_freed = substr(a, checkpoint + 1, index->i - checkpoint - 1);
	if (ft_strlen(to_be_freed) && index->j != 0 && \
	theres_is_red(index->new[index->j - 1]))
		index->new[index->j++] = ft_strdup(to_be_freed);
	else if (!ft_strlen(to_be_freed) && index->j != 0 && \
	theres_is_red(index->new[index->j - 1]))
		index->new[index->j++] = ft_strdup(to_be_freed);
	else if (ft_strlen(to_be_freed))
		index->new[index->j++] = ft_strdup(to_be_freed);
	else if (index->j != 0 && !theres_is_red(index->new[index->j - 1]))
		index->new[index->j++] = NULL;
	else
		index->new[index->j++] = ft_calloc(1, 1);
	free(to_be_freed);
	shel->status[shel->stat++] = 0;
}

void	handel_double_quotes(t_mshel *shel, char *a, t_indexs *index, int cp)
{
	char	*tp;
	char	*to_be_freed;

	index->i++;
	while (a[index->i] != 34 && a[index->i])
		index->i++;
	if (index->j != 0 && index->new[index->j - 1] && \
	!ft_strcmp(index->new[index->j - 1], "<<"))
		shel->exapnd_herdoc[shel->herdoc_number++] = 0;
	to_be_freed = substr(a, cp, index->i - cp);
	if (ft_strchr(to_be_freed, '$') && a[index->i - 1] != '$')
		d_quote_expand(a, cp, index, shel);
	else
	{
		if (cp != 0 && a[cp - 1] != ' ' && a[cp - 1] != 9)
		{
			tp = substr(a, cp + 1, index->i - cp - 1);
			d_quotes_j(tp, index->new, shel, &index->j);
			free(tp);
		}
		else
			d_quote_copy(shel, index, cp, a);
	}
	free(to_be_freed);
}
