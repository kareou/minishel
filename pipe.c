/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 22:48:51 by mkhairou          #+#    #+#             */
/*   Updated: 2023/04/19 18:22:51 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_redirect_place(int in, int out)
{
	if(in == 1 && out == 1)
		return (1);
	else if(in == 1)
		return (2);
	else if(out == 1)
		return (3);
	else
		return (0);
}

int redirect_to_pipe(t_mshel *shel , int (*pipe)[2], int i, int red_status, int status)
{
	if (i == 0)
	{
		if(red_status == 2)
		{
			if(!redirect_input(shel,i, 0))
				return (0);
		}
		if(red_status == 3)
		{
			if(!redirect_output(shel,i))
				return (0);
		}
		if(red_status != 3 && status != 0)
		{
			if (dup2(pipe[i][1], STDOUT_FILENO) == -1)
				perror("minishell :");
		}
	}
	if (i > 0)
	{
		if (i + 1 < shel->cmd_number)
		{
			if(check_redirect_place(shel->cmd[i - 1]->redirect.in,shel->cmd[i - 1]->redirect.out) == 3)
				redirect_input(shel,i - 1, 1);
			if(red_status != 2)
			{
				if (dup2(pipe[i - 1][0], STDIN_FILENO) == -1)
					perror("minishell :");
			}
			if(red_status == 3)
				redirect_output(shel,i);
			if (red_status != 3)
			{
				if (dup2(pipe[i][1], STDOUT_FILENO) == -1)
					perror("minishell :");
			}
		}
		else
		{
			if(check_redirect_place(shel->cmd[i - 1]->redirect.in,shel->cmd[i - 1]->redirect.out) == 3)
				redirect_input(shel,i - 1, 1);
			if(red_status == 3)
				redirect_output(shel,i);
			if(check_redirect_place(shel->cmd[i - 1]->redirect.in,shel->cmd[i - 1]->redirect.out) != 3)
			{
				if (dup2(pipe[i - 1][0], STDIN_FILENO) == -1)
					perror("minishell :");
			}
		}

	}
	return (1);
}

void close_all_pipes(int (*pipe)[2], int cmd_numbers)
{
	int i;

	i = 0;
	while (i < cmd_numbers - 1)
	{
		close(pipe[i][0]);
		close(pipe[i][1]);
		i++;
	}
}

void pipe_and_start(t_mshel *mshel)
{
	int i;
	int id;
	int	pid[mshel->cmd_number];
	int pipes[mshel->cmd_number][2];

	i = 0;
	while (i < mshel->cmd_number - 1)
	{
		if (pipe(pipes[i]) == -1)
			printf("minishell : %s\n", strerror(errno));
		i++;
	}
	i = 0;
	while (i < mshel->cmd_number)
	{
		id = fork();
		if (id == -1)
			printf("minishell : %s\n", strerror(errno));
		if (id == 0)
			execute_cmd(mshel, pipes, i, 1);
		else
			pid[i] = id;
		i++;
	}
	close_all_pipes(pipes, mshel->cmd_number);
	i = 0;
	while (i < mshel->cmd_number)
	{
		waitpid(pid[i], &mshel->exit_status, 0);
		i++;
	}

}
