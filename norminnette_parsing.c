/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norminnette_parsing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 11:55:25 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/11 21:30:02 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	hendel_no_quotes(t_mshel *shel,char **new, int *j, char *tmp)
{

	int s = 0;
	new[(*j)++] = parsse_redirection(tmp, &s);
	shel->status[shel->stat++] = 0;
	if (tmp[s])
	{
		if ((*j) != 0 && new[(*j) - 1] && !strcmp(new[(*j) - 1], "<<"))
		{
			shel->exapnd_herdoc[shel->herdoc_number] = 1;
			shel->herdoc_number++;
		}
		new[(*j)++] = check_expanding(shel,substr(tmp, s, ft_strlen(tmp) - (s )));
		shel->status[shel->stat++] = 1;
	}
}

void	handel_sing_quote(t_mshel *shel, char *a, int *i,char **new, int *j)
{
	int	checkpoint;

	checkpoint = (*i);
	(*i)++;
	while (a[(*i)] != 39 && a[(*i)])
		(*i)++;
	if ((*j) != 0 && new[(*j) - 1] && !strcmp(new[(*j) - 1], "<<"))
	{
		shel->exapnd_herdoc[shel->herdoc_number] = 0;
		shel->herdoc_number++;
	}
	if (checkpoint != 0 && a[checkpoint - 1] != ' ')
	{
		char *store = substr(a, checkpoint + 1, (*i) - checkpoint - 1);
		if(!store[0] && new[(*j) - 1][ft_strlen(new[(*j) - 1]) - 1] == ' ' && !a[(*i) + 1])
		{
			shel->status[shel->stat - 1] = 0;
			new[(*j) - 1] = ft_strtrim(new[(*j) - 1], " ");
		}
		else
		{
			if(ft_strchr(new[(*j) - 1],' ') && check_space_place(new[(*j) - 1]) == 1)
				new[(*j) - 1] = ft_strjoin(ft_strtr(new[(*j) - 1], " "), store);
			else
				new[(*j) - 1] = ft_strjoin(new[(*j) - 1], store);
		}
	}
	else
	{
		if (ft_strlen(substr(a, checkpoint + 1, (*i) - checkpoint - 1)) && (*j) != 0 && theres_is_red(new[(*j) - 1]))
			new[(*j)++] = substr(a, checkpoint + 1, (*i) - checkpoint - 1);
		else if (!ft_strlen(substr(a, checkpoint + 1, (*i) - checkpoint - 1)) && (*j) != 0 && theres_is_red(new[(*j) - 1]))
			new[(*j)++] = substr(a, checkpoint + 1, (*i) - checkpoint - 1);
		else if (ft_strlen(substr(a, checkpoint + 1, (*i) - checkpoint - 1)))
			new[(*j)++] = substr(a, checkpoint + 1, (*i) - checkpoint - 1);
		else if ((*j) != 0 && !theres_is_red(new[(*j) - 1]))
			new[(*j)++] = NULL;
		else
			new[(*j)++] = ft_calloc(1, 1);
		shel->status[shel->stat++] = 0;
	}
}

void	handel_double_quotes(t_mshel *shel, char *a, int *i, int *j, char **new)
{
	int	checkpoint;

	checkpoint = *i;
	(*i)++;
	while (a[*i] != 34 && a[*i])
		(*i)++;
	if ((*j) != 0 && new[(*j) - 1] && !strcmp(new[(*j) - 1], "<<"))
	{
		shel->exapnd_herdoc[shel->herdoc_number] = 0;
		shel->herdoc_number++;
	}
	if (ft_strchr(substr(a, checkpoint, (*i) - checkpoint), '$') && a[(*i) - 1] != '$')
	{
		char *tet = check_expanding(shel, substr(a, checkpoint + 1, (*i) - 1 - checkpoint));
		if (checkpoint != 0 && a[checkpoint - 1] != ' ')
		{
			if (tet)
				new[(*j) - 1] = ft_strjoin(new[(*j) - 1], tet);
		}
		else
		{
			if (tet)
				new[(*j)++] = strdup(tet);
			else
				new[(*j)++] = NULL;
		}
		shel->status[shel->stat++] = 0;
	}
	else
	{
		if (checkpoint != 0 && a[checkpoint - 1] != ' ')
		{
			if ((*j) != 0)
			{
				char *tp = substr(a, checkpoint + 1, (*i) - checkpoint - 1);
				if(new[(*j) -  1][0] && tp[0])
				{
					if(ft_strchr(new[(*j) - 1], ' ') && check_space_place(new[(*j) - 1]) == 1 && shel->status[shel->stat - 1] == 1)
						new[(*j) - 1] = ft_strjoin(ft_strtr(new[(*j) - 1], " "), substr(a, checkpoint + 1, (*i) - checkpoint - 1));
					else
						new[(*j) - 1] = ft_strjoin(new[(*j) - 1], substr(a, checkpoint + 1, (*i) - checkpoint - 1));
				}
				else
				{
					if(tp[0])
						new[(*j) - 1] = ft_strdup(tp);
					else if(!new[(*j) - 1][0])
						new[(*j) - 1] = NULL;
				}
			}
			else
			{
				char *tp = substr(a, checkpoint + 1, (*i) - checkpoint - 1);
				if(tp)
					new[(*j)++] = substr(a, checkpoint + 1, (*i) - checkpoint - 1);
				else
					new[(*j)++] = ft_calloc(1,1);
			}
		}
		else
		{
			if (ft_strlen(substr(a, checkpoint + 1, (*i) - checkpoint - 1)) && (*j) != 0 && theres_is_red(new[(*j) - 1]))
				new[(*j)++] = substr(a, checkpoint + 1, (*i) - checkpoint - 1);
			else if (!ft_strlen(substr(a, checkpoint + 1, (*i) - checkpoint - 1)) && (*j) != 0 && theres_is_red(new[(*j) - 1]))
				new[(*j)++] = substr(a, checkpoint + 1, (*i) - checkpoint - 1);
			else if (ft_strlen(substr(a, checkpoint + 1, (*i) - checkpoint - 1)))
				new[(*j)++] = substr(a, checkpoint + 1, (*i) - checkpoint - 1);
			else if ((*j) != 0 && !theres_is_red(new[(*j) - 1]))
				new[(*j)++] = NULL;
			else
				new[(*j)++] = ft_calloc(1, 1);
			shel->status[shel->stat++] = 0;
		}
	}
}


void	handel_no_quotes_expand(char *tempo, char **new, int *j, char a, t_mshel *shel)
{
	if (tempo)
	{
		if (new[(*j) - 1])
		{
			if(ft_strlen(tempo) > 1)
			{
				if(a)
					new[(*j) - 1] = ft_strjoin(new[(*j) - 1], tempo);
				else
				{
					if(new[(*j) - 1][0])
						new[(*j) - 1] = ft_strjoin(new[(*j) - 1], ft_strtrim(tempo, " "));
					else
						new[(*j) - 1] = ft_strtr(tempo, " ");
				}
			}
			if(ft_strchr(tempo,' '))
				shel->status[shel->stat - 1] = 1;
		}
		else
		{
			if (ft_strlen(tempo))
			{
				if ((*j) != 0)
				{
					char *l = ft_strtr(tempo," ");
					if(l[0])
						new[(*j) - 1] = l;
					else
					{
						if(!a)
							new[(*j) - 1] = tempo;
						else
						new[(*j) - 1] = l;
					}
					if(ft_strchr(tempo, ' ') && !a)
						shel->status[shel->stat - 1] = 1;
				}
				else
				{
					if(a)
						new[(*j)++] = ft_strdup(tempo);
					else
						new[(*j)++] = ft_strtrim(tempo, " ");
					if(ft_strchr(tempo, ' ') && !a)
						shel->status[shel->stat - 1] = 1;
				}
			}
			else
			{
				new[(*j) - 1] = NULL;
				shel->status[shel->stat - 1] = 1;
			}
		}
	}
	if ((*j) != 0 && new[(*j) - 1] && !strcmp(new[(*j) - 1], "<<"))
	{
		shel->exapnd_herdoc[shel->herdoc_number] = 1;
		shel->herdoc_number++;
	}
}

void	hendel_no_quotes_spand_j(t_mshel *shel , char *tempo, char **new, int *j, char a)
{
	if (tempo && ft_strchr(tempo, ' '))
	{
		if ((*j) != 0 && !strcmp(new[(*j) - 1], "<<"))
		{
			shel->exapnd_herdoc[shel->herdoc_number] = 1;
			shel->herdoc_number++;
		}
		char **t = ft_split(tempo, ' ');
		if (array_lenth(t) == 1)
		{
			if(a == ' ')
				new[(*j)++] = ft_strtr(tempo, " ");
			else
			{
				if(!a)
				{
					new[(*j)++] = ft_strdup(t[0]);
				}
				else
					new[(*j)++] = ft_strdup(tempo);
			}
			shel->status[shel->stat++] = 1;
		}
		else
		{
			if ((*j) != 0 && !theres_is_red(new[(*j) - 1]))
			{
				new[(*j)++] = ft_strdup(tempo);
				shel->status[shel->stat++] = 1;
			}
			else
			{
				int index = 0;
				if ((*j) != 0 && !strcmp(new[(*j) - 1], "<<"))
					shel->exapnd_herdoc[shel->herdoc_number] = 1;
				if (!t[index])
					new[(*j)++] = ft_calloc(1, 1);
				else
				{
					while (t[index])
					{
						new[(*j)++] = ft_strtrim(t[index], " ");
						shel->status[shel->stat++] = 1;
						index++;
					}
				}
			}
		}
	}
	else if (tempo)
	{
		if ((*j) != 0 && !strcmp(new[(*j) - 1], "<<"))
		{
			shel->exapnd_herdoc[shel->herdoc_number] = 1;
			shel->herdoc_number++;
		}
		new[(*j)++] = ft_strdup(tempo);
		shel->status[shel->stat++] = 1;
	}
	else
	{
		if ((*j) != 0 && !strcmp(new[(*j) - 1], "<<"))
		{
			shel->exapnd_herdoc[shel->herdoc_number] = 1;
			shel->herdoc_number++;
		}
		new[(*j)++] = ft_calloc(1, 1);
		shel->status[shel->stat++] = 1;
	}
}

void	hande_no_quoet_expand_n(t_mshel *shel, char **new, int *j, char *a, int checkpoint, int i)
{
	char *tmp = substr(a, checkpoint, i - checkpoint);
	if (ft_strchr(tmp, '<') || ft_strchr(tmp, '>'))
		{
			int s = 0;
			new[(*j)++] = parsse_redirection(tmp, &s);
			if (tmp[s])
			{
				if ((*j) != 0 && new[(*j) - 1] && !strcmp(new[(*j) - 1], "<<"))
				{
					shel->exapnd_herdoc[shel->herdoc_number] = 1;
					shel->herdoc_number++;
				}
				new[(*j)++] = ft_strtrim(substr(tmp, s, ft_strlen(tmp) - (s)), " ");
				shel->status[shel->stat++] = 0;
			}
			shel->status[shel->stat++] = 0;
		}
	else
	{
		if ((*j) != 0 && !strcmp(new[(*j) - 1], "<<"))
		{
			shel->exapnd_herdoc[shel->herdoc_number] = 1;
			shel->herdoc_number++;
		}
		if (checkpoint != 0 && a[checkpoint - 1] != ' ')
			new[(*j) - 1] = ft_strjoin(new[(*j) - 1], substr(a, checkpoint, i - checkpoint));
		else
		{
			new[(*j)++] = substr(a, checkpoint, i - checkpoint);
			shel->status[shel->stat++] = 0;
		}
	}
}
