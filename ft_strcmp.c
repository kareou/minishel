/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 20:12:14 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/20 12:04:52 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(char *a, char *b)
{
	int	i;

	i = 0;
	if ((!a && !b))
		return (-1);
	if (!a || !a[0])
		return (-1);
	while (a[i] || b[i])
	{
		if (a[i] != b[i])
		{
			if (a[i])
				return (a[i] - b[i]);
			else
				return (-b[i]);
		}
		i++;
	}
	return (0);
}
