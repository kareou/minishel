/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 20:18:41 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/22 15:10:48 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dupenv(t_mshel *shel, char *variable, char **new_env)
{
	int	i;

	i = 0;
	while (shel->env[i])
	{
		new_env[i] = ft_strdup(shel->env[i]);
		i++;
	}
	new_env[i] = ft_strdup(variable);
	i++;
	new_env[i] = NULL;
	free_array(shel->env);
}

int	add_env(t_mshel *shel, char *variable)
{
	int		i;
	char	**var;
	char	**new_env;

	i = 0;
	var = ft_split(variable, '=');
	if (!check_valid(var[0], 1, shel))
		return (0);
	while (shel->env[i])
	{
		if (!ft_strncmp(shel->env[i], var[0], ft_strlen(var[0]))
			&& shel->env[i][ft_strlen(var[0])] == '=')
		{
			free(shel->env[i]);
			free_array(var);
			shel->env[i] = ft_strdup(variable);
			return (1);
		}
		i++;
	}
	new_env = malloc(sizeof(char *) * (i + 2));
	dupenv(shel, variable, new_env);
	shel->env = new_env;
	free_array(var);
	return (1);
}
