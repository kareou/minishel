/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 17:55:09 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/19 19:39:11 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strtr(char *s1, char *set)
{
	int		i;
	int		len;
	char	*a;

	i = 0;
	if (!s1 || !set)
		return (NULL);
	while (*s1 && ft_strchr(set, *s1))
		s1++;
	len = ft_strlen(s1) - 1;
	while (len > 0 && ft_strchr(set, s1[len]))
		len--;
	a = ft_substr(s1, 0, len + 1);
	return (a);
}

void	no_redirection(char *tmp, t_mshel *shel, t_indexs *idx, int checkpoint)
{
	char	*trimed;
	char	*tempo;

	trimed = ft_strtrim(tmp, " ");
	tempo = check_expanding(shel, trimed);
	free(trimed);
	if (checkpoint != 0 && idx->a[checkpoint - 1] != ' ')
		handel_no_quotes_expand(tempo, idx, idx->a[(idx->i)],
			shel);
	else
		hendel_no_quotes_spand_j(shel, tempo, idx,
			idx->a[(idx->i)]);
	if (tempo)
		free(tempo);
}

void	no_quot_part(char *a, t_mshel *shel, t_indexs *index)
{
	char	*tmp;
	int		checkpoint;

	checkpoint = index->i;
	index->a = a;
	while (a[(index->i)] != '\0' && a[(index->i)] != 39 && a[(index->i)] != 34
		&& a[(index->i)] != ' ')
		(index->i)++;
	if ((index->i) > checkpoint)
	{
		tmp = substr(a, checkpoint, (index->i) - checkpoint);
		if (ft_strchr(tmp, '$'))
		{
			if (ft_strchr(tmp, '<') || ft_strchr(tmp, '>'))
				hendel_no_quotes(shel, index->new, &index->j, tmp);
			else
				no_redirection(tmp, shel, index, checkpoint);
		}
		else
			hande_no_quoet_expand_n(shel, index, a, checkpoint);
		free(tmp);
	}
}

char	*substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*a;

	i = 0;
	if (!s)
		return (0);
	j = ft_strlen(s);
	a = (char *)malloc(len + 1);
	if (!a)
		return (NULL);
	while (s[i] && i < len && start < j)
	{
		a[i] = s[i + start];
		i++;
	}
	a[i] = '\0';
	return (a);
}
