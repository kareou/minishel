/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 10:56:24 by mkhairou          #+#    #+#             */
/*   Updated: 2023/04/19 20:01:23 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void string_to_print(char *a)
{
	int i;
	char *tmp;
	int j;

	i = -1;
	// if (shel->singl_c == 1)
	// {
	//     while (a[++i])
	//         printf("%c", a[i]);
	// }
	// else
	// {
		while (a[++i])
		{
			if (a[i] == 36)
			{
				j = i;
				while (a[++i] != ' ' && a[++i]);
				tmp = getenv(ft_substr(a, j+1, i));
				if(tmp)
					printf("%s", tmp);
			}
			printf("%c", a[i]);
		}
	// }
}

void    ech_o(t_mshel *shel, int i)
{
	int j;

	j = 0;
	while (shel->cmd[i]->args[j])
	{
		if(shel->cmd[i]->flags == 1)
			j++;
		string_to_print(shel->cmd[i]->args[j]);
		j++;
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
