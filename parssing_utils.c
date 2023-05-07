/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parssing_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:52:28 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/07 22:09:04 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*parsse_redirection(char *input, int *s)
{
	char	*new;
	while ((input[*s] == '>' || input[*s] == '<') && input[*s])
		(*s)++;
	if (*s > 0)
		new = ft_strtrim(substr(input, 0, *s), " ");
	if (input[*s] == '>')
	{
		if (input[*s + 1] == '>')
		{
			new = ">>";
			(*s)++;
		}
		else
			new = ">";
	}
	else if (input[*s] == '<')
	{
		if (input[*s + 1] == '<')
		{
			new = "<<";
			(*s)++;
		}
		else
			new = "<";
	}
	return(new);
}
