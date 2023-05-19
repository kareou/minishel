/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 11:17:55 by asekkak           #+#    #+#             */
/*   Updated: 2023/05/19 14:29:25 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*str;
	int		i;

	str = (char *)s;
	i = 0;
	i = strlen(str);
	while (i >= 0)
	{
		if (str[i] == (unsigned char)c)
			return (&(str[i]));
		i--;
	}
	if (c == 0)
		return (&str[i]);
	return (0);
}
