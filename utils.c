/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 13:25:37 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/22 15:45:10 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_space_place(char *a)
{
	int	i;

	i = 1;
	if (a[0] == ' ' && a[ft_strlen(a)] == ' ')
		return (0);
	if (a[0] == ' ')
		return (1);
	else
		return (2);
}

int	array_lenth(char **a)
{
	int	i;

	i = 0;
	while (a[i])
		i++;
	return (i);
}

int	theres_is_red(char *a)
{
	if (!a)
		return (1);
	if (!ft_strcmp(a, "<") || !ft_strcmp(a, "<<") || !ft_strcmp(a, ">")
		|| !ft_strcmp(a, ">>"))
		return (0);
	return (1);
}

void	print_errors(char *a)
{
	write(2, a, ft_strlen(a));
	write(2, "\n", 1);
}

char	**better_parsing(char *a, t_mshel *shel, t_indexs *index)
{
	int		checkpoint;

	shel->stat = 0;
	index->new = ft_calloc(1024, 1024);
	shel->herdoc_number = 0;
	while (a[index->i])
	{
		checkpoint = index->i;
		if (a[index->i] != 39 && a[index->i] != 34)
			no_quot_part(a, shel, index);
		checkpoint = index->i;
		if (a[index->i] == 39 || a[index->i] == 34)
		{
			if (a[index->i] == 39)
				handel_sing_quote(shel, a, index);
			else if (a[index->i] == 34)
				handel_double_quotes(shel, a, index);
		}
		if (a[index->i] == 0)
			break ;
		index->i++;
	}
	index->new[index->j] = NULL;
	return (index->new);
}
