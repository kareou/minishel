/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 13:53:15 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/24 16:22:27 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_file(int i, char *a)
{
	int	state;

	state = 0;
	while (i)
	{
		if (ft_isalnum(a[i]) || a[i] == '\'' || a[i] == '\"')
			state = 1;
		i--;
	}
	return (state);
}

int	print_error(int cho)
{
	if (cho == 0)
		print_errors("minishell: syntax error ");
	else if (cho == 1)
		print_errors("minishell: unexpected EOF while looking for matching");
	return (0);
}

int	check_er(char *a, int i, int c)
{
	int	count;

	count = 0;
	while (a[i] && a[i] == c)
	{
		count++;
		i++;
	}
	while (a[i] && a[i] == ' ')
	{
		i++;
	}
	if (count > 2 || a[i] == '>' || a[i] == '<' || a[i] == '|')
		return (0);
	return (1);
}

int	check_syntax2(char *a, int i, int *prev_symbol)
{
	if (i == 0 || a[i + 1] == '\0' || a[i - 1] == '|')
	{
		if (i == 0 && (a[i] == '<' || a[i] == '>'))
			if (!check_er(a, i, a[i]))
				return (0);
		if (i == 0 && a[i] != '<' && a[i] != '>')
			return (0);
		else if (!a[i + 1])
			return (0);
	}
	else if (a[i] == '<' && a[i + 1] == '>')
		return (0);
	else if (a[i] == '>' && a[i + 1] == '<')
		return (0);
	else if (a[i] == '|' && !find_file(i, a))
		return (0);
	else if ((a[i] == '>' || a[i] == '<'))
	{
		if (!check_er(a, i, a[i]))
			return (0);
	}
	else
		*prev_symbol = 1;
	return (1);
}

int	check_syntax(char *a, int i, int quotes)
{
	int	prev_symbol;

	prev_symbol = 0;
	while (a[++i])
	{
		if (a[i] == ' ' && !a[i + 1] && !quotes && prev_symbol)
			return (print_error(0));
		if (a[i] == '"' || a[i] == '\'')
		{
			if (!quotes)
				quotes = a[i];
			else if (quotes == a[i])
				quotes = 0;
		}
		else if (!quotes && (a[i] == '<' || a[i] == '>' || a[i] == '|'))
		{
			if (!check_syntax2(a, i, &prev_symbol))
				return (print_error(0));
		}
		else
			prev_symbol = 0;
	}
	if (quotes != 0)
		return (print_error(1));
	return (1);
}
