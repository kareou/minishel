/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 16:44:47 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/24 16:52:42 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	expand_normal(char **tmp, t_mshel *shel, char **value, int i)
{
	int		k;
	char	*buffer;
	char	*placeholder;

	k = 0;
	while (tmp[i][k] && (ft_isalnum(tmp[i][k]) || tmp[i][k] == '_'))
		k++;
	buffer = ft_substr(tmp[i], 0, k);
	(*value) = smart_join((*value), ft_getenv(shel, buffer));
	free(buffer);
	if (tmp[i][k])
	{
		placeholder = ft_substr(tmp[i], k, ft_strlen(tmp[i]));
		(*value) = smart_join((*value), placeholder);
		free(placeholder);
	}
}

void	expande_dollare_s(int i, char **value, char **tmp, t_mshel *shel)
{
	char	*ext;

	(*value) = ft_strdup(tmp[0]);
	while (tmp[i])
	{
		if (ft_isdigit(tmp[i][0]))
			(*value) = smart_join((*value), tmp[i] + 1);
		if (tmp[i][0] == '?')
		{
			ext = ft_itoa(shel->exit_status);
			(*value) = smart_join((*value), ext);
			free(ext);
			if (tmp[i][1])
				(*value) = smart_join((*value), tmp[i] + 1);
		}
		else
			expand_normal(tmp, shel, value, i);
		i++;
	}
}
