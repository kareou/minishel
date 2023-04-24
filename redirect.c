/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 18:48:02 by mkhairou          #+#    #+#             */
/*   Updated: 2023/04/24 13:29:47 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_to_last_output(t_mshel *mshel, int cmd_index)
{
	int	i;
	int	fd;

	i = 0;
	while (mshel->cmd[cmd_index]->redirect.output[i])
	{
		fd = open(mshel->cmd[cmd_index]->redirect.out_file[i], O_RDONLY);
		if(fd == -1)
		{
			perror("minisell :");
			exit(1);
		}
		if (!mshel->cmd[cmd_index]->redirect.output[i + 1])
		{
			mshel->cmd[cmd_index]->redirect.old_input = dup(STDIN_FILENO);
			if (dup2(fd,STDIN_FILENO) == 1)
				perror("minishell :");
		}
		close(fd);
		i++;
	}
}

int	redirect_input(t_mshel *mshel, int cmd_index, int status)
{
	int	fd;
	int	i;

	i = 0;
	if(status)
		redirect_to_last_output(mshel, cmd_index);
	else
	{
		while (mshel->cmd[cmd_index]->redirect.input[i])
		{
			fd = open(mshel->cmd[cmd_index]->redirect.in_file[i], O_RDONLY);
			if(fd == -1)
			{
				perror("minisell :");
				exit(1);
			}
			if (!mshel->cmd[cmd_index]->redirect.input[i + 1])
			{
				if (dup2(fd,STDIN_FILENO) == 1)
					perror("minishell :");
			}
			close(fd);
			i++;
		}
	}
	return(1);
}

int	redirect_output(t_mshel *mshel, int cmd_index)
{
	int	tmp_fd;
	int	i;

	i = 0;
	while (mshel->cmd[cmd_index]->redirect.output[i])
	{
		if(!strcmp(mshel->cmd[cmd_index]->redirect.output[i], ">>"))
		{
			tmp_fd = open(mshel->cmd[cmd_index]->redirect.out_file[i], O_RDWR | O_CREAT | O_APPEND, 0777);
			if(tmp_fd == -1)
			{
				perror("minishell :");
				return(0);
			}
		}
		else
		{
			tmp_fd = open(mshel->cmd[cmd_index]->redirect.out_file[i], O_RDWR  | O_CREAT | O_TRUNC, 0644);
			if(tmp_fd == -1)
			{
				perror("minishell :");
					return(0);
			}
		}
		if (!mshel->cmd[cmd_index]->redirect.output[i + 1])
		{
			if (dup2(tmp_fd,STDOUT_FILENO) == -1)
				perror("minishell :");
		}
		close(tmp_fd);
		i++;
	}
	return(1);
}
