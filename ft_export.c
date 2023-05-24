/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 20:24:39 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/24 12:00:28 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_valid_extand(char *a, int declare, int i)
{
	if (!ft_isalpha(a[0]) && a[0] != '_')
	{
		if (declare)
			print_errors("minishell: not a valid identifier");
		return (0);
	}
	else
	{
		i++;
		if (a[i])
		{
			while (a[i])
			{
				if (!ft_isalnum(a[i]) && a[i] != '_')
				{
					if (declare)
						print_errors("minishell: not a valid identifier");
					return (0);
				}
				i++;
			}
		}
	}
	return (1);
}

int	check_valid(char *a, int declare, t_mshel *shel)
{
	int	i;

	(void)shel;
	i = 1;
	if (!a)
	{
		if (declare)
			print_errors("minishell: not a valid identifier");
		return (0);
	}
	if (!check_valid_extand(a, declare, 0))
		return (0);
	return (1);
}

void	free_array(char **a)
{
	int	i;

	i = 0;
	while (a[i])
	{
		free(a[i]);
		i++;
	}
	free(a);
}

int	ft_export(t_mshel *shel, int cmd_index, int i)
{
	int	state;

	state = 0;
	if (!shel->cmd[cmd_index]->args[i + 1])
		print_env(shel, 1);
	else
	{
		while (shel->cmd[cmd_index]->args[++i])
		{
			if (ft_strchr(shel->cmd[cmd_index]->args[i], '=')
				&& (ft_strchr(shel->cmd[cmd_index]->args[i], '=') + 1))
			{
				if (!add_env(shel, shel->cmd[cmd_index]->args[i], -1))
					state = 1;
				if (!add_x_env(shel, shel->cmd[cmd_index]->args[i]))
					state = 1;
			}
			else
			{
				if (!add_x_env(shel, shel->cmd[cmd_index]->args[i]))
					state = 1;
			}
		}
	}
	return (state);
}
