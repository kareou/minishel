/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 01:25:51 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/05 22:56:58 by mkhairou         ###   ########.fr       */
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
	int		i;

	i = 0;
	while (a[i])
	{
		write(fd, &a[i],1);
		i++;
	}
	write(fd,"\n",1);
}

void	ft_heredoc(int cmd_index, t_mshel *shel)
{
	int	h_number = shel->cmd[cmd_index]->redirect.heredoc.heredoc_number;
	int	du;
	int	duin;
	int pipes[h_number][2];
	int	i = 0;
	int	tmp_fd;
	char *holder;
	int	redirection;
	char *a;
	holder = NULL;
	open_n_close_p(pipes, 0, h_number);
	redirection = check_redirect_place(shel->cmd[cmd_index]->redirect.in, shel->cmd[cmd_index]->redirect.out);
	if(shel->cmd_number > 1 || (redirection && shel->cmd[cmd_index]->error == -1))
	{
		if ((redirection == 2 || redirection == 1 ))
		{
			duin = dup(STDIN_FILENO);
			dup2(shel->cmd[cmd_index]->redirect.old_input, STDIN_FILENO);
		}
		du = dup(STDOUT_FILENO);
		dup2(shel->cmd[cmd_index]->redirect.old_output, STDOUT_FILENO);
	}
	while (1)
	{
		a = readline("heredoc> ");
		if (a == NULL)
			break;
		if(a[0] == '$')
			holder = check_expanding(shel,a);
		if(!strcmp(a,shel->cmd[cmd_index]->redirect.heredoc.delemiter[i]) || (holder  && !strcmp(holder + 1,shel->cmd[cmd_index]->redirect.heredoc.delemiter[i])))
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
		{
			if ((a[0] == '$' || (a[0] == '\'' || a[0] == '"' &&  && a[0] == '$')) shel->exapnd_herdoc[i] == 1)
				printf_in_pipe(check_expanding(shel, a),pipes[i][1]);
			else
				printf_in_pipe(a,pipes[i][1]);
		}
		free(a);
	}
	if(shel->cmd_number > 1 || (redirection && shel->cmd[cmd_index]->error == -1))
	{
		if ((redirection == 2 || redirection == 1) && cmd_index == 0)
		{
			dup2(duin, STDIN_FILENO);
			close(duin);
		}
		dup2(du, STDOUT_FILENO);
		close(du);
	}
	if (redirection != 2 && redirection != 1 && shel->cmd[cmd_index]->error == -1)
	{
		tmp_fd = dup(STDIN_FILENO);
		if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
				perror("minisdhell :");
	}
	open_n_close_p(pipes, 1, h_number);
	if(shel->cmd[cmd_index]->cmd && shel->cmd[cmd_index]->cmd[0] && shel->cmd[cmd_index]->error == -1)
		run_cmd(shel, cmd_index, shel->cmd[cmd_index]->cmd);
	if (shel->cmd[cmd_index]->redirect.old_input != 0 && (redirection != 2 || redirection != 1) && shel->cmd[cmd_index]->error == -1)
		dup2(tmp_fd, STDIN_FILENO);
}

