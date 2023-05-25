/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 22:48:51 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/25 11:13:13 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_redirect_place(int in, int out)
{
	if (in == 1 && out == 1)
		return (1);
	else if (in == 1)
		return (2);
	else if (out == 1)
		return (3);
	else
		return (0);
}

void	check_herdoc(t_mshel *mshel, int (*pipes)[2])
{
	int	i;
	int	id;

	i = 0;
	while (i < mshel->cmd_number)
	{
		if (mshel->cmd[i]->redirect.heredoc.heredoc_number > 0)
		{
			id = fork();
			if (id == -1)
				printf("minishell : %s\n", strerror(errno));
			if (id == 0)
				execute_cmd(mshel, pipes, i, 1);
			else
			{
				mshel->id = id;
				wait(NULL);
			}
		}
		i++;
	}
}

void	create_proc(t_mshel *mshel, int (*pipes)[2], int *pid)
{
	int	i;
	int	id;

	i = 0;
	while (i < mshel->cmd_number)
	{
		if (mshel->cmd[i]->redirect.heredoc.heredoc_number > 0)
			i++;
		else
		{
			id = fork();
			if (id == -1)
				printf("minishell : %s\n", strerror(errno));
			if (id == 0)
				execute_cmd(mshel, pipes, i, 1);
			else
				pid[i] = id;
		}
		i++;
	}
}

void	pipe_and_start(t_mshel *mshel)
{
	int	i;
	int	status;
	int	*pid;
	int	(*pipes)[2];

	i = 0;
	status = 0;
	pid = malloc(sizeof(int) * mshel->cmd_number);
	pipes = malloc(sizeof(int *) * mshel->cmd_number);
	open_n_close_p(pipes, 0, mshel->cmd_number - 1);
	check_herdoc(mshel, pipes);
	create_proc(mshel, pipes, pid);
	open_n_close_p(pipes, 1, mshel->cmd_number - 1);
	i = 0;
	while (i < mshel->cmd_number)
	{
		waitpid(pid[i], &status, 0);
		i++;
	}
	mshel->exit_status = WEXITSTATUS(status);
	free(pid);
	free(pipes);
}
