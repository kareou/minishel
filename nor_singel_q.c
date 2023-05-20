/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nor_singel_q.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 22:17:41 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/20 22:22:40 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	join_sing_q(t_mshel *shel, t_indexs *index, char *a, int checkpoint)
{
	char	*store;

	store = substr(a, checkpoint + 1, index->i - checkpoint - 1);
	if (!store[0] && index->new[index->j - 1][ft_strlen(index->new[index->j
				- 1]) - 1] == ' ' && !a[index->i + 1])
	{
		shel->status[shel->stat - 1] = 0;
		index->new[index->j - 1] = ft_strtrim(index->new[index->j - 1],
				" ");
	}
	else
	{
		if (ft_strchr(index->new[index->j - 1], ' ')
			&& check_space_place(index->new[index->j - 1]) == 1)
			index->new[index->j
				- 1] = ft_strjoin(ft_strtr(index->new[index->j - 1], " "),
					store);
		else
			index->new[index->j - 1] = ft_strjoin(index->new[index->j - 1],
					store);
	}
}

void	no_join_sing(t_mshel *shel, t_indexs *index, char *a, int checkpoint)
{
	char	*tmp;

	tmp = substr(a, checkpoint + 1, index->i - checkpoint - 1);
	if (ft_strlen(tmp) && index->j != 0 && theres_is_red(index->new[index->j
				- 1]))
		index->new[index->j++] = tmp;
	else if (!ft_strlen(tmp) && index->j != 0
		&& theres_is_red(index->new[index->j - 1]))
		index->new[index->j++] = tmp;
	else if (ft_strlen(tmp))
		index->new[index->j++] = tmp;
	else if (index->j != 0 && !theres_is_red(index->new[index->j - 1]))
		index->new[index->j++] = NULL;
	else
		index->new[index->j++] = ft_calloc(1, 1);
	shel->status[shel->stat++] = 0;
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
	if (checkpoint != 0 && a[checkpoint - 1] != ' ')
		join_sing_q(shel, index, a, checkpoint);
	else
		no_join_sing(shel, index, a, checkpoint);
}
