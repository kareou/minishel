/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parssing_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:52:28 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/22 12:34:13 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

char	*second_part(char *input, int *s)
{
	char	*new;

	if (input[*s + 1] == '<')
	{
		new = ft_strdup("<<");
		(*s)++;
	}
	else
		new = ft_strdup("<");
	(*s)++;
	return (new);
}

char	*parsse_redirection(char *input, int *s)
{
	char	*new;
	char	*tmp;

	if (*s > 0)
	{
		tmp = substr(input, 0, *s);
		new = ft_strtrim(tmp, " ");
		free(tmp);
	}
	if (input[*s] == '>')
	{
		if (input[*s + 1] == '>')
		{
			new = ft_strdup(">>");
			(*s)++;
		}
		else
			new = ft_strdup(">");
		(*s)++;
	}
	else if (input[*s] == '<')
		new = second_part(input, s);
	return (new);
}
