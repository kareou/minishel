/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 10:56:24 by mkhairou          #+#    #+#             */
/*   Updated: 2023/04/27 15:30:19 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void string_to_print(char *a, t_mshel *shel)
{
	int i;
	char *tmp;
	int j;

	i = -1;
	while (a[++i])
	{
		if (a[i] == 36)
		{
			if (a[i + 1] == '?')
			{
				tmp = ft_itoa(shel->exit_status);
				++i;
			}
			else
			{
				j = i;
				while (a[++i] != ' ' && a[++i]);
				tmp = getenv(ft_substr(a, j+1, i));
			}
			if(tmp)
				printf("%s", tmp);
		}
		else
			printf("%c", a[i]);
	}
}

void    ech_o(t_mshel *shel, int i)
{
	int j;

	j = 0;
	while (shel->cmd[i]->args[j])
	{
		if(shel->cmd[i]->flags == 1)
			j++;
		string_to_print(shel->cmd[i]->args[j], shel);
		j++;
		if(shel->cmd[i]->args[j])
			printf(" ");
	}
	if(shel->cmd[i]->flags != 1)
		printf("\n");
	else
		printf("%%");
}

void print_env(t_mshel *shel, int stat)
{
	int i;

	i = 0;
	if(!stat)
	{
		while (shel->env[i])
		{
			printf("%s\n", shel->env[i]);
			i++;
		}
	}
	else
	{
		while (shel->x_env[i])
		{
			printf("declare -x \"%s\"\n", shel->x_env[i]);
			i++;
		}
	}
}
