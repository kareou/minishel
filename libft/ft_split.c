/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 13:23:02 by asekkak           #+#    #+#             */
/*   Updated: 2023/05/19 14:30:37 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

int	c_count(const char *s, const char c)
{
	size_t	i;
	int		trgr;
	int		count;

	i = 0;
	trgr = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && trgr == 0)
		{
			count++;
			trgr = 1;
		}
		if (s[i] == c && trgr > 0)
			trgr = 0;
		i++;
	}
	return (count);
}

void	init(int *i, int *j, int *trgr)
{
	*i = 0;
	*j = 0;
	*trgr = 0;
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	int		trgr;
	char	**a;

	if (!s)
		return (NULL);
	init(&i, &j, &trgr);
	a = (char **)malloc(sizeof(char *) * (c_count(s, c) + 1));
	if (a == NULL)
		return (NULL);
	while (s[i])
	{
		trgr = i;
		while (s[i] != '\0' && s[i] != c)
			i++;
		if (i != 0)
			a[j++] = ft_substr(s, trgr, i - trgr);
		while (s[i] == c && s[i] != '\0')
			i++;
	}
	a[j] = NULL;
	return (a);
}
