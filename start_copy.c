/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_copy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 17:41:16 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/23 15:17:18 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dup_env(t_mshel *shel, char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	shel->env = malloc(sizeof(char *) * (i + 1));
	shel->x_env = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env[i])
	{
		shel->x_env[i] = ft_strdup(env[i]);
		shel->env[i] = ft_strdup(env[i]);
		i++;
	}
	shel->x_env[i] = NULL;
	shel->env[i] = NULL;
}

void	alloc_all(t_mshel *mshel, int size_arrays, t_lexer *lexer)
{
	t_lexer	*head;
	int		i;

	head = lexer;
	i = 0;
	while (i < size_arrays)
	{
		mshel->cmd[i] = malloc(sizeof(t_cmd));
		mshel->cmd[i]->args = malloc(sizeof(char *) * \
		(array_lenth(head->str) + 1));
		mshel->cmd[i]->error = -1;
		mshel->cmd[i]->redirect.heredoc.heredoc_number = 0;
		mshel->cmd[i]->redirect.in = 0;
		mshel->cmd[i]->redirect.out = 0;
		mshel->cmd[i]->redirect.old_output = 0;
		mshel->cmd[i]->redirect.old_input = 0;
		mshel->cmd[i]->redirect.ambugius = 0;
		mshel->cmd[i]->red = 0;
		i++;
		if (head->next)
			head = head->next;
	}
}

void	transfer_to_array(t_lexer *lexer, int size_arrays, t_mshel *mshel)
{
	int	pipe[0][2];

	mshel->cmd = malloc(sizeof(t_cmd *) * size_arrays);
	if (!mshel->cmd)
		exit(1);
	alloc_all(mshel, size_arrays, lexer);
	transfer_cmd(lexer, mshel, 0);
	free_lexer(lexer);
	if (mshel->cmd_number == 1)
		execute_cmd(mshel, pipe, 0, 0);
	else
		pipe_and_start(mshel);
	free_all(mshel, size_arrays);
}
