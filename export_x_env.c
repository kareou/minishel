/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_x_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 20:26:28 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/22 15:10:57 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	start_adding(t_mshel *shel, char *variable, int *i)
{
	char	**var;

	var = ft_split(variable, '=');
	if (!var[1] && variable[ft_strlen(variable) - 1] != '=')
	{
		if (!check_valid(var[0], 1, shel))
			return (0);
	}
	else
	{
		if (!check_valid(var[0], 0, shel))
			return (0);
	}
	while (shel->x_env[(*i)])
	{
		if (!ft_strncmp(shel->x_env[(*i)], var[0], ft_strlen(var[0])))
		{
			free(shel->x_env[(*i)]);
			free_array(var);
			shel->x_env[(*i)] = ft_strdup(variable);
			return (1);
		}
		(*i)++;
	}
	free_array(var);
	return (2);
}

int	add_x_env(t_mshel *shel, char *variable)
{
	int		i;
	int		state;
	char	**new_env;

	i = 0;
	state = start_adding(shel, variable, &i);
	if (state != 2)
		return (state);
	new_env = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (shel->x_env[i])
	{
		new_env[i] = ft_strdup(shel->x_env[i]);
		i++;
	}
	new_env[i] = ft_strdup(variable);
	i++;
	new_env[i] = NULL;
	free_array(shel->x_env);
	shel->x_env = new_env;
	return (1);
}
