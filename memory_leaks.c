/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_leaks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:59:02 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/25 11:12:57 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_args(char **a)
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

void	free_all(t_mshel *shel, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(shel->cmd[i]->cmd);
		free_args(shel->cmd[i]->args);
		if (shel->cmd[i]->red)
		{
			free_args(shel->cmd[i]->redirect.input);
			free_args(shel->cmd[i]->redirect.in_file);
			free(shel->cmd[i]->redirect.input_expanded);
			free_args(shel->cmd[i]->redirect.output);
			free_args(shel->cmd[i]->redirect.out_file);
			free(shel->cmd[i]->redirect.heredoc.cmd);
			free(shel->cmd[i]->redirect.output_expanded);
			free_array(shel->cmd[i]->redirect.heredoc.delemiter);
		}
		free(shel->cmd[i]);
		i++;
	}
	free(shel->cmd);
	free(shel->status);
	free(shel->exapnd_herdoc);
}

int	count_array(char **a, char *c, char *d)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (a[i])
	{
		if (!ft_strcmp(a[i], c) || !ft_strcmp(a[i], d))
			count++;
		i++;
	}
	return (count + 1);
}

int	word_num(char *a, int c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (a[i])
	{
		if (a[i] == c)
			count++;
		i++;
	}
	return (count + 1);
}

void	free_lexer(t_lexer *lexer)
{
	t_lexer	*head;

	while (lexer)
	{
		free_args(lexer->str);
		free(lexer->table);
		free(lexer->env);
		head = lexer;
		lexer = lexer->next;
		free(head);
	}
}
