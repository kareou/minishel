/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nor_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 21:40:08 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/20 21:47:10 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	join(t_mshel *shel, char *tempo, t_indexs *index, char a)
{
	char	*tmp;

	if (ft_strlen(tempo) > 1)
	{
		if (a)
			index->new[index->j - 1] = ft_strjoin(index->new[index->j
					- 1], tempo);
		else
		{
			if (index->new[index->j - 1][0])
			{
				tmp = ft_strtrim(tempo, " ");
				index->new[index->j
					- 1] = ft_strjoin(index->new[index->j - 1], tmp);
			}
			else
				index->new[index->j - 1] = ft_strtr(tempo, " ");
		}
	}
	if (ft_strchr(tempo, ' '))
		shel->status[shel->stat - 1] = 1;
}

void	no_join(t_mshel *shel, char *tempo, t_indexs *index, char a)
{
	char	*l;

	if (index->j != 0)
	{
		l = ft_strtr(tempo, " ");
		if (l[0])
			index->new[index->j - 1] = l;
		else
		{
			if (!a)
				index->new[index->j - 1] = ft_strdup(tempo);
			else
				index->new[index->j - 1] = l;
		}
	}
	else
	{
		if (a)
			index->new[index->j++] = ft_strdup(tempo);
		else
			index->new[index->j++] = ft_strtrim(tempo, " ");
	}
	if (ft_strchr(tempo, ' ') && !a)
			shel->status[shel->stat - 1] = 1;
}

void	handel_no_quotes_expand(char *tempo, t_indexs *index, char a,
		t_mshel *shel)
{
	if (tempo)
	{
		if (index->new[index->j - 1])
			join(shel, tempo, index, a);
		else
		{
			if (ft_strlen(tempo))
				no_join(shel, tempo, index, a);
			else
			{
				index->new[index->j - 1] = NULL;
				shel->status[shel->stat - 1] = 1;
			}
		}
	}
	if (index->j != 0 && index->new[index->j - 1]
		&& !ft_strcmp(index->new[index->j - 1], "<<"))
	{
		shel->exapnd_herdoc[shel->herdoc_number] = 1;
		shel->herdoc_number++;
	}
}
