/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 11:33:12 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/25 21:55:14 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_utils(char *a)
{
	int	i;

	i = 0;
	if (a[i] == '-' || a[i] == '+')
		i++;
	while (a[i])
	{
		if (!ft_isdigit(a[i]))
			return (0);
		i++;
	}
	return (1);
}

int	check_multip(char **a, t_mshel *shel)
{
	int	i;

	i = 0;
	while (a[i])
	{
		if (i > 0)
		{
			print_errors("minishell: exit: too many arguments");
			if (shel->cmd_number == 1)
				return (1);
			else
				exit(1);
		}
		i++;
	}
	return (0);
}

void	exiting(t_mshel *shel, int exit_status, int free_all_flag)
{
	free_array(shel->env);
	free_array(shel->x_env);
	if (free_all_flag)
		free_all(shel, shel->cmd_number);
	exit(exit_status);
}

int	check_exit_num(char **a, t_mshel *shel)
{
	char	*tmp;
	char	*error;

	if (!exit_utils(a[0]))
	{
		tmp = ft_strjoin("minishell: exit: ", a[0]);
		error = ft_strjoin(tmp, ": numeric argument required");
		print_errors(error);
		free(tmp);
		free(error);
		exiting(shel, 255, 1);
	}
	return (0);
}

int	exit_function(char **a, t_mshel *shel)
{
	int		i;

	i = 0;
	if (!a[0])
		exiting(shel, shel->exit_status, 1);
	else
	{
		check_exit_num(a, shel);
		if (check_multip(a, shel) == 1)
			return (1);
		i = 0;
		while (a[i])
		{
			if (ft_isdigit(a[i][0]) || (a[i][0] == '-' && ft_isdigit(a[i][1])))
				exiting(shel, ft_atoi(a[i]) % 256, 1);
			i++;
		}
	}
	return (0);
}
