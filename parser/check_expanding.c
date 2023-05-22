/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_expanding.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 10:58:22 by asekkak           #+#    #+#             */
/*   Updated: 2023/05/22 15:33:41 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

char	*smart_join(char *a, char *b)
{
	char *p;
	p = a;
	a = ft_strjoin(a, b);
	free(p);
	return (a);
}

void	expande_dollare_s(int i, char **value, char **tmp, t_mshel *shel)
{
	int	k;
	char	*buffer;
	char	*placeholder;

	(*value) = ft_strdup(tmp[0]);
	while (tmp[i])
	{
		if (ft_isdigit(tmp[i][0]))
			(*value) = smart_join((*value), tmp[i] + 1);
		if (tmp[i][0] == '?')
		{
			(*value) = smart_join((*value), ft_itoa(shel->exit_status));
			if (tmp[i][1])
				(*value) = smart_join((*value), tmp[i] + 1);
		}
		else
		{
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
		i++;
	}
}

void	expand_dollar_f(char **tmp, t_mshel *shel, int i, char **value)
{
	int		k;
	char	*temporary;
	char	*placeholder;

	k = 0;
	while (tmp[i][k] && (ft_isalnum(tmp[i][k]) || tmp[i][k] == '_'))
		k++;
	temporary = ft_substr(tmp[i], 0, k);
	placeholder = ft_getenv(shel, temporary);
	free(temporary);
	if (placeholder)
	{
		if ((*value))
			(*value) = ft_strjoin((*value), placeholder);
		else
		{
			if (placeholder && placeholder[0])
				(*value) = ft_strdup(placeholder);
		}
	}
	else
		(*value) = NULL;
	if (tmp[i][k])
		(*value) = ft_strjoin((*value), ft_substr(tmp[i], k,
					ft_strlen(tmp[i])));
}

void	expand_exit(t_mshel *shel, char **value, char **tmp, int i)
{
	char	*placeholder;
	char	*tmp2;

	placeholder = ft_itoa(shel->exit_status);
	if ((*value))
	{
		tmp2 = (*value);
		(*value) = ft_strjoin((*value), placeholder);
		free(tmp2);
	}
	else
		(*value) = ft_strdup(placeholder);
	if (tmp[i][1])
	{
		tmp2 = (*value);
		(*value) = ft_strjoin((*value), tmp[i] + 1);
		free(tmp2);
	}
	free(placeholder);
}

void	free_split(char **tmp)
{
	int	i;

	i = 0;
	while (tmp[i])
	{
		free(tmp[i]);
		i++;
	}
	free(tmp);
}

char	*check_expanding(t_mshel *shel, char *str)
{
	int		i;
	int		dollar_index;
	char	**tmp;
	char	*value;

	i = 1;
	dollar_index = find_dollar(str);
	if (!str[dollar_index + 1])
		return (ft_strdup(str));
	tmp = ft_split(str, '$');
	value = NULL;
	if (dollar_index == 0)
	{
		i = -1;
		while (tmp[++i])
		{
			if (ft_isdigit(tmp[i][0]) || (!ft_isalnum(tmp[i][0])
					&& (tmp[i][0] != '?' && tmp[i][0] != '_')))
				value = ft_substr(tmp[i], 1, ft_strlen(tmp[i]));
			else if (tmp[i][0] == '?')
				expand_exit(shel, &value, tmp, i);
			else
				expand_dollar_f(tmp, shel, i, &value);
		}
	}
	else if (dollar_index > 0 && tmp)
		expande_dollare_s(i, &value, tmp, shel);
	free_split(tmp);
	return (value);
}
