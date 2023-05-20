/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 01:25:51 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/20 19:46:28 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(char *a, int c)
{
	char	*returned;

	if (c == '\'')
		returned = ft_strtrim(a, "'");
	if (c == '"')
		returned = ft_strtrim(a, "\"");
	return (returned);
}

void	reset_red(t_mshel *shel, int cmd_index, t_indexs *indexs, \
int redirection)
{
	if (shel->cmd_number > 1 || (redirection && \
	shel->cmd[cmd_index]->error == -1))
	{
		if ((redirection == 2 || redirection == 1) && cmd_index == 0)
		{
			dup2(indexs->duin, STDIN_FILENO);
			close(indexs->duin);
		}
		dup2(indexs->du, STDOUT_FILENO);
		close(indexs->du);
	}
}

void	run_cmd_h(t_mshel *shel, int cmd_index, t_indexs *indexs,
				int (*pipes)[2])
{
	int	tmp_fd;
	int	h_number;
	int	redirection;

	redirection = check_redirect_place(shel->cmd[cmd_index]->redirect.in,
			shel->cmd[cmd_index]->redirect.out);
	h_number = shel->cmd[cmd_index]->redirect.heredoc.heredoc_number;
	reset_red(shel, cmd_index, indexs, redirection);
	if (redirection != 2 && redirection != 1 && \
	shel->cmd[cmd_index]->error == -1)
	{
		tmp_fd = dup(STDIN_FILENO);
		if (indexs->i != 0)
		{
			if (dup2(pipes[indexs->i - 1][0], STDIN_FILENO) == -1)
				perror("minisdhell :");
		}
	}
	open_n_close_p(pipes, 1, h_number);
	if (shel->cmd[cmd_index]->cmd && shel->cmd[cmd_index]->error == -1)
		run_cmd(shel, cmd_index, shel->cmd[cmd_index]->cmd);
	if (shel->cmd[cmd_index]->redirect.old_input != 0 && (redirection != 2 \
	|| redirection != 1) && shel->cmd[cmd_index]->error == -1)
		dup2(tmp_fd, STDIN_FILENO);
}

void	cancell_red(t_mshel *shel, int c_idx, int redirection, t_indexs *idx)
{
	if ((redirection == 2 || redirection == 1))
	{
		idx->duin = dup(STDIN_FILENO);
		dup2(shel->cmd[c_idx]->redirect.old_input, STDIN_FILENO);
	}
	idx->du = dup(STDOUT_FILENO);
	dup2(shel->cmd[c_idx]->redirect.old_output, STDOUT_FILENO);
}

void	ft_heredoc(int cmd_index, t_mshel *shel)
{
	int			h_number;
	t_indexs	indexs;
	int			(*pipes)[2];
	char		*holder;
	int			redirection;

	h_number = shel->cmd[cmd_index]->redirect.heredoc.heredoc_number;
	pipes = malloc(sizeof(int [2]) * h_number);
	indexs.du = 0;
	indexs.duin = 0;
	indexs.i = 0;
	holder = NULL;
	open_n_close_p(pipes, 0, h_number);
	redirection = check_redirect_place(shel->cmd[cmd_index]->redirect.in,
			shel->cmd[cmd_index]->redirect.out);
	if (shel->cmd_number > 1 || \
	(redirection && shel->cmd[cmd_index]->error == -1))
		cancell_red(shel, cmd_index, redirection, &indexs);
	read_line(shel, cmd_index, &indexs.i, pipes);
	run_cmd_h(shel, cmd_index, &indexs, pipes);
}
