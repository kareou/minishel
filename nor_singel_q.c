/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nor_singel_q.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 22:17:41 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/24 16:20:49 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	join_sing_q(t_mshel *shel, t_indexs *idx, char *a, int checkpoint)
{
	char	*store;
	char	*point;
	char	*tmp;

	store = substr(a, checkpoint + 1, idx->i - checkpoint - 1);
	point = ft_strtr(idx->new[idx->j - 1], " ");
	if (!store[0] && idx->new[idx->j - 1][ft_strlen(idx->new[idx->j
				- 1]) - 1] == ' ' && !a[idx->i + 1])
	{
		shel->status[shel->stat - 1] = 0;
		tmp = idx->new[idx->j - 1];
		idx->new[idx->j - 1] = ft_strtrim(idx->new[idx->j - 1], " ");
		free(tmp);
	}
	else
	{
		if (ft_strchr(idx->new[idx->j - 1], ' ')
			&& check_space_place(idx->new[idx->j - 1]) == 1)
			idx->new[idx->j - 1] = ft_strjoin(point, store);
		else
			idx->new[idx->j - 1] = smart_join(idx->new[idx->j - 1],
					store);
	}
	return (free(store), free(point));
}

void	no_join_sing(t_mshel *shel, t_indexs *index, char *a, int checkpoint)
{
	char	*tmp;

	tmp = substr(a, checkpoint + 1, index->i - checkpoint - 1);
	if (ft_strlen(tmp) && index->j != 0 && theres_is_red(index->new[index->j
				- 1]))
		index->new[index->j++] = ft_strdup(tmp);
	else if (!ft_strlen(tmp) && index->j != 0
		&& theres_is_red(index->new[index->j - 1]))
		index->new[index->j++] = ft_strdup(tmp);
	else if (ft_strlen(tmp))
		index->new[index->j++] = ft_strdup(tmp);
	else if (index->j != 0 && !theres_is_red(index->new[index->j - 1]))
		index->new[index->j++] = NULL;
	else
		index->new[index->j++] = ft_calloc(1, 1);
	shel->status[shel->stat++] = 0;
	free(tmp);
}

void	handel_sing_quote(t_mshel *shel, char *a, t_indexs *index)
{
	int		checkpoint;

	checkpoint = index->i;
	index->i++;
	while (a[index->i] != 39 && a[index->i])
		index->i++;
	if (index->j != 0 && index->new[index->j - 1]
		&& !ft_strcmp(index->new[index->j - 1], "<<"))
	{
		shel->exapnd_herdoc[shel->herdoc_number] = 0;
		shel->herdoc_number++;
	}
	if (checkpoint != 0 && a[checkpoint - 1] != ' ' && a[checkpoint - 1] != 9)
		join_sing_q(shel, index, a, checkpoint);
	else
		no_join_sing(shel, index, a, checkpoint);
}
