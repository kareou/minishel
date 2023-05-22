/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 11:22:57 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/22 15:55:45 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	start_redirect_pipe(t_mshel *shel, int cmd_index, \
int status, int (*pipe)[2])
{
	int	red_status;

	red_status = check_redirect_place(shel->cmd[cmd_index]->redirect.in, \
	shel->cmd[cmd_index]->redirect.out);
	if (!redirect_to_pipe(shel, pipe, cmd_index, red_status, status))
	{
		if (!status && \
		shel->cmd[cmd_index]->redirect.heredoc.heredoc_number == 0)
		{
			if (shel->cmd[cmd_index]->error == -3)
				print_errors("minishell: ambiguous redirect");
			else
				error_to_print(shel->cmd[cmd_index]->error, \
				shel->cmd[cmd_index]->error_file);
			shel->exit_status = 1;
			return (1);
		}
		else if (status)
			exit(1);
	}
	return (0);
}

void	cancel_red(t_mshel *shel, int cmd_index)
{
	if (shel->cmd[cmd_index]->redirect.old_output != 0)
		dup2(shel->cmd[cmd_index]->redirect.old_output, \
		STDOUT_FILENO);
	if (shel->cmd[cmd_index]->redirect.old_input != 0)
	{
		if (dup2(shel->cmd[cmd_index]->redirect.old_input, STDIN_FILENO) == -1)
		perror("minishell :");
	}
}

int	redirect_in_out(t_mshel *shel, int cmd_index, int status)
{
	int	error;

	error = 0;
	if (!redirect_input(shel, cmd_index, 0))
		error = shel->cmd[cmd_index]->error;
	if (!redirect_output(shel, cmd_index))
		error = shel->cmd[cmd_index]->error;
	if (error)
	{
		if (error == -3)
			print_errors("minishell: ambiguous redirect");
		else
			error_to_print(error, shel->cmd[cmd_index]->error_file);
		shel->exit_status = 1;
		if (status)
			exit(1);
		else
		{
			cancel_red(shel, cmd_index);
			return (1);
		}
	}
	return (0);
}

int	reset_redirection(t_mshel *shel, int cmd_index, int status)
{
	if (shel->cmd[cmd_index]->error != -1)
	{
		if (shel->cmd[cmd_index]->error == -3)
			print_errors("minishell: ambiguous redirect");
		else
			error_to_print(shel->cmd[cmd_index]->error, \
			shel->cmd[cmd_index]->error_file);
		shel->exit_status = 1;
		if (status)
			exit(1);
		else
			return (1);
	}
	if (shel->cmd[cmd_index]->redirect.old_output != 0)
	{
		if (dup2(shel->cmd[cmd_index]->redirect.old_output, \
		STDOUT_FILENO) == -1)
			perror("minishell :");
	}
	if (shel->cmd[cmd_index]->redirect.old_input != 0)
	{
		if (dup2(shel->cmd[cmd_index]->redirect.old_input, STDIN_FILENO) == -1)
			perror("minishell :");
	}
	return (0);
}

void	execute_cmd(t_mshel *shel, int (*pipe)[2], int cmd_index, int status)
{
	int		red_status;

	red_status = check_redirect_place(shel->cmd[cmd_index]->redirect.in, \
	shel->cmd[cmd_index]->redirect.out);
	if (start_redirect_pipe(shel, cmd_index, status, pipe))
		return ;
	if (red_status == 1)
	{
		if (redirect_in_out(shel, cmd_index, status))
			return ;
	}
	if (red_status != 1 && status != 0)
		open_n_close_p(pipe, 1, shel->cmd_number - 1);
	if (shel->cmd[cmd_index]->cmd)
	{
		if (shel->cmd[cmd_index]->redirect.heredoc.heredoc_number == 0)
			run_cmd(shel, cmd_index, shel->cmd[cmd_index]->cmd);
	}
	if (shel->cmd[cmd_index]->redirect.heredoc.heredoc_number > 0)
		ft_heredoc(cmd_index, shel);
	if (shel->cmd_number > 1)
		exit(0);
	if (reset_redirection(shel, cmd_index, status))
		return ;
}
