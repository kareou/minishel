/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:11:16 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/19 19:52:51 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirecrect_m(int red_status, t_mshel *shel, int (*pipe)[2], int i)
{
	if (red_status == 2)
	{
		if (!redirect_input(shel, i, 0))
			return (0);
	}
	if (red_status != 2)
	{
		if (dup2(pipe[i - 1][0], STDIN_FILENO) == -1)
			perror("minishell :");
	}
	if (red_status == 3)
		redirect_output(shel, i);
	if (red_status != 3)
	{
		if (dup2(pipe[i][1], STDOUT_FILENO) == -1)
			perror("minishell :");
	}
	return (1);
}

int	redirect_l(int red_status, t_mshel *shel, int (*pipe)[2], int i)
{
	if (red_status == 2)
	{
		if (!redirect_input(shel, i, 0))
			return (0);
	}
	if (red_status != 2)
	{
		if (dup2(pipe[i - 1][0], STDIN_FILENO) == -1)
			perror("minishell :");
	}
	if (red_status == 3)
		redirect_output(shel, i);
	return (1);
}

int	redirect_to_pipe(t_mshel *shel, int (*pipe)[2], int i, int red_status,
		int status)
{
	shel->cmd[i]->redirect.old_input = dup(STDIN_FILENO);
	shel->cmd[i]->redirect.old_output = dup(STDOUT_FILENO);
	if (i == 0)
	{
		if (red_status == 2)
		{
			if (!redirect_input(shel, i, 0))
				return (0);
		}
		if (red_status == 3)
		{
			if (!redirect_output(shel, i))
				return (0);
		}
		if (red_status != 3 && status != 0)
			dup2(pipe[i][1], STDOUT_FILENO);
	}
	if (i > 0)
	{
		if (i + 1 < shel->cmd_number)
			redirecrect_m(red_status, shel, pipe, i);
		else
			redirect_l(red_status, shel, pipe, i);
	}
	return (1);
}
