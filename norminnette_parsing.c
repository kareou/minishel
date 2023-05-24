/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norminnette_parsing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 11:55:25 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/22 16:53:36 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handel_red_sing_exp(t_mshel *shel, t_indexs *index, char *tmp, int *s)
{
	int		checkpoint;
	char	*holder;

	checkpoint = *s;
	while (tmp[*s] != '>' && tmp[*s] != '<' && tmp[*s] != '\0')
		(*s)++;
	if ((*s) != checkpoint)
	{
		holder = substr(tmp, checkpoint, *s - checkpoint);
		if (ft_strchr(holder, '$'))
		{
			index->new[index->j++] = check_expanding(shel, holder);
			shel->status[shel->stat++] = 1;
		}
		else
		{
			index->new[index->j++] = \
			ft_substr(tmp, checkpoint, *s - checkpoint);
			shel->status[shel->stat++] = 0;
		}
	}
	index->new[index->j++] = parsse_redirection(tmp, s);
	shel->status[shel->stat++] = 0;
}

void	hendel_no_quotes(t_mshel *shel, t_indexs *index, char *tmp)
{
	int		s;
	char	*pointer;

	s = 0;
	handel_red_sing_exp(shel, index, tmp, &s);
	if (tmp[s])
	{
		if (index->j != 0 && index->new[index->j - 1] && \
		!ft_strcmp(index->new[index->j - 1], "<<"))
		{
			shel->exapnd_herdoc[shel->herdoc_number] = 1;
			shel->herdoc_number++;
		}
		pointer = substr(tmp, s, ft_strlen(tmp) - (s));
		index->new[index->j++] = check_expanding(shel, pointer);
		free(pointer);
		shel->status[shel->stat++] = 1;
	}
}

void	handel_red_singel(t_mshel *shel, t_indexs *index, char *tmp, int *s)
{
	int		checkpoint;

	checkpoint = *s;
	while (tmp[*s] != '>' && tmp[*s] != '<' && tmp[*s] != '\0')
		(*s)++;
	if ((*s) != checkpoint)
	{
		index->new[index->j++] = ft_substr(tmp, checkpoint, *s - checkpoint);
		shel->status[shel->stat++] = 0;
	}
	index->new[index->j++] = parsse_redirection(tmp, s);
	shel->status[shel->stat++] = 0;
}

void	hane_no_q_exp_part2(t_mshel *shel, t_indexs *index, char *a, \
int checkpoint)
{
	char	*to_befreed;
	char	*poin;

	to_befreed = substr(a, checkpoint, index->i - checkpoint);
	if (index->j != 0 && !ft_strcmp(index->new[index->j - 1], "<<"))
		shel->exapnd_herdoc[shel->herdoc_number++] = 1;
	if (checkpoint != 0 && a[checkpoint - 1] != ' ' && a[checkpoint - 1] != 9)
	{
		poin = index->new[index->j - 1];
		index->new[index->j - 1] = ft_strjoin(index->new[index->j - 1],
				to_befreed);
		free(to_befreed);
		free(poin);
	}
	else
	{
		index->new[index->j++] = to_befreed;
		shel->status[shel->stat++] = 0;
	}
}

void	hande_no_quoet_expand_n(t_mshel *shel, t_indexs *index, char *a,
		int checkpoint)
{
	char	*tmp;
	int		s;
	char	*to_befreed;

	tmp = substr(a, checkpoint, index->i - checkpoint);
	if (ft_strchr(tmp, '<') || ft_strchr(tmp, '>'))
	{
		s = 0;
		handel_red_singel(shel, index, tmp, &s);
		if (tmp[s])
		{
			if (index->j != 0 && index->new[index->j - 1]
				&& !ft_strcmp(index->new[index->j - 1], "<<"))
				shel->exapnd_herdoc[shel->herdoc_number++] = 1;
			to_befreed = substr(tmp, s, ft_strlen(tmp) - (s));
			index->new[index->j++] = ft_strtrim(to_befreed, " ");
			shel->status[shel->stat++] = 0;
			free(to_befreed);
		}
	}
	else
		hane_no_q_exp_part2(shel, index, a, checkpoint);
	free(tmp);
}
