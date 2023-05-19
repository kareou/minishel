/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_leaks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:59:02 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/14 16:57:27 by mkhairou         ###   ########.fr       */
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
		if (shel->cmd[i]->redirect.in || shel->cmd[i]->redirect.out)
		{
			free_args(shel->cmd[i]->redirect.input);
			free_args(shel->cmd[i]->redirect.in_file);
			free(shel->cmd[i]->redirect.input_expanded);
			free_args(shel->cmd[i]->redirect.output);
			free_args(shel->cmd[i]->redirect.out_file);
			free(shel->cmd[i]->redirect.heredoc.cmd);
			free(shel->cmd[i]->redirect.output_expanded);
			free(shel->cmd[i]->redirect.heredoc.delemiter);
		}
		free(shel->cmd[i]);
		i++;
	}
	free(shel->cmd);
}

void	free_lexer(t_lexer *lexer)
{
	t_lexer	*head;

	while (lexer)
	{
		free_args(lexer->str);
		free(lexer->table);
		head = lexer;
		lexer = lexer->next;
		free(head);
	}
}
