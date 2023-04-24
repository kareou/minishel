/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 01:25:51 by mkhairou          #+#    #+#             */
/*   Updated: 2023/04/24 13:33:41 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_n_close_p(int (*pipes)[2], int cs, int p_number)
{
	int	i;

	i = 0;
	if(cs == 0)
	{
		while (i < p_number)
		{
			pipe(pipes[i]);
			i++;
		}
	}
	else
	{
		while (i < p_number)
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
			i++;
		}
	}

}
void	printf_in_pipe(char *a, int fd)
{
	int i;

	i = 0;
	while (a[i])
	{
		write(fd, &a[i],1);
		i++;
	}
	write(fd,"\n",1);
}

void	ft_heredoc(char *cmd , char *dilemter, int cmd_index, t_mshel *shel)
{
	int	h_number = shel->cmd[cmd_index]->redirect.heredoc.heredoc_number;
	int	du;
	int pipes[h_number][2];
	int	i = 0;
	int	tmp_fd;
	int	id;
	char *a;

	open_n_close_p(pipes, 0, h_number);
	(void)dilemter;
	(void)cmd;
	if(shel->cmd_number > 1)
	{
		du = dup(STDOUT_FILENO);
		dup2(shel->cmd[cmd_index]->redirect.old_output, STDOUT_FILENO);
		close(shel->cmd[cmd_index]->redirect.old_output);
	}
	while (1)
	{
		a = readline("heredoc> ");
		if(!strcmp(a,shel->cmd[cmd_index]->redirect.heredoc.delemiter[i]))
		{
			a = NULL;
			i++;
		}
		if(i >= shel->cmd[cmd_index]->redirect.heredoc.heredoc_number)
		{
			free(a);
			break;
		}
		if(a)
			printf_in_pipe(a,pipes[i][1]);
		free(a);
	}
	if (shel->cmd_number > 1)
	{
		dup2(du, STDOUT_FILENO);
		close(du);
	}

	tmp_fd = dup(STDIN_FILENO);
	if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
				perror("minishell :");
	open_n_close_p(pipes, 1, h_number);
	if(shel->cmd_number >= 1)
	{
		id = fork();
		if ( id == 0)
		{
			if(shel->cmd[cmd_index]->redirect.heredoc.cmd[0])
				execute_shell(shel->cmd[cmd_index]->redirect.heredoc.cmd, shel);
		}
		else
			wait(NULL);
	}
	else
	{
		if(shel->cmd[cmd_index]->redirect.heredoc.cmd[0])
				execute_shell(shel->cmd[cmd_index]->redirect.heredoc.cmd, shel);
	}
	dup2(tmp_fd, STDIN_FILENO);
}

