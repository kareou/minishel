/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norminnette_parsing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 11:55:25 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/19 21:10:39 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	hendel_no_quotes(t_mshel *shel, char **new, int *j, char *tmp)
{
	int		s;
	char	*pointer;

	s = 0;
	new[(*j)++] = parsse_redirection(tmp, &s);
	shel->status[shel->stat++] = 0;
	if (tmp[s])
	{
		if ((*j) != 0 && new[(*j) - 1] && !ft_strcmp(new[(*j) - 1], "<<"))
		{
			shel->exapnd_herdoc[shel->herdoc_number] = 1;
			shel->herdoc_number++;
		}
		pointer = substr(tmp, s, ft_strlen(tmp) - (s));
		new[(*j)++] = check_expanding(shel, pointer);
		free(pointer);
		shel->status[shel->stat++] = 1;
	}
}

void	handel_sing_quote(t_mshel *shel, char *a, t_indexs *index)
{
	int		checkpoint;
	char	*store;
	char	*tmp;

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
	{
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
	else
	{
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
}

void	handel_no_quotes_expand(char *tempo, t_indexs *index, char a,
		t_mshel *shel)
{
	char	*tmp;
	char	*l;

	if (tempo)
	{
		if (index->new[index->j - 1])
		{
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
		else
		{
			if (ft_strlen(tempo))
			{
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
					if (ft_strchr(tempo, ' ') && !a)
						shel->status[shel->stat - 1] = 1;
				}
				else
				{
					if (a)
						index->new[index->j++] = ft_strdup(tempo);
					else
						index->new[index->j++] = ft_strtrim(tempo, " ");
					if (ft_strchr(tempo, ' ') && !a)
						shel->status[shel->stat - 1] = 1;
				}
			}
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

void	hendel_no_quotes_spand_j(t_mshel *shel, char *tempo, t_indexs *index,
		char a)
{
	char	**t;
	int		ind;

	if (tempo && ft_strchr(tempo, ' '))
	{
		if (index->j != 0 && !ft_strcmp(index->new[index->j - 1], "<<"))
		{
			shel->exapnd_herdoc[shel->herdoc_number] = 1;
			shel->herdoc_number++;
		}
		t = ft_split(tempo, ' ');
		if (array_lenth(t) == 1)
		{
			if (a == ' ')
				index->new[index->j++] = ft_strtr(tempo, " ");
			else
			{
				if (!a)
				{
					index->new[index->j++] = ft_strdup(t[0]);
				}
				else
					index->new[index->j++] = ft_strdup(tempo);
			}
			shel->status[shel->stat++] = 1;
		}
		else
		{
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
	}
	else if (tempo)
	{
		if (index->j != 0 && !ft_strcmp(index->new[index->j - 1], "<<"))
		{
			shel->exapnd_herdoc[shel->herdoc_number] = 1;
			shel->herdoc_number++;
		}
		index->new[index->j++] = ft_strdup(tempo);
		shel->status[shel->stat++] = 1;
	}
	else
	{
		if (index->j != 0 && !ft_strcmp(index->new[index->j - 1], "<<"))
		{
			shel->exapnd_herdoc[shel->herdoc_number] = 1;
			shel->herdoc_number++;
		}
		index->new[index->j++] = ft_calloc(1, 1);
		shel->status[shel->stat++] = 1;
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
		index->new[index->j++] = parsse_redirection(tmp, &s);
		if (tmp[s])
		{
			if (index->j != 0 && index->new[index->j - 1]
				&& !ft_strcmp(index->new[index->j - 1], "<<"))
			{
				shel->exapnd_herdoc[shel->herdoc_number] = 1;
				shel->herdoc_number++;
			}
			to_befreed = substr(tmp, s, ft_strlen(tmp) - (s));
			index->new[index->j++] = ft_strtrim(to_befreed, " ");
			shel->status[shel->stat++] = 0;
			free(to_befreed);
		}
		shel->status[shel->stat++] = 0;
	}
	else
	{
		to_befreed = substr(a, checkpoint, index->i - checkpoint);
		if (index->j != 0 && !ft_strcmp(index->new[index->j - 1], "<<"))
		{
			shel->exapnd_herdoc[shel->herdoc_number] = 1;
			shel->herdoc_number++;
		}
		if (checkpoint != 0 && a[checkpoint - 1] != ' ')
		{
			index->new[index->j - 1] = ft_strjoin(index->new[index->j - 1],
					to_befreed);
			free(to_befreed);
		}
		else
		{
			index->new[index->j++] = to_befreed;
			shel->status[shel->stat++] = 0;
		}
	}
	free(tmp);
}
