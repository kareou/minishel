/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 18:48:02 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/22 12:49:53 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_to_print(int error, char *file)
{
	char	*msg;
	char	*tp;
	char	*tmp;

	if (error == 13)
	{
		tp = ft_strjoin(": ", strerror(error));
		msg = ft_strjoin(file, tp);
		free(tp);
	}
	else
		msg = strerror(error);
	tmp = ft_strjoin("minishell: ", msg);
	free(file);
	print_errors(tmp);
	free(tmp);
}

int	check_error_n_red(int fd, int cmd_index, t_mshel *mshel, int i)
{
	if (fd == -1)
	{
		mshel->cmd[cmd_index]->error = errno;
		mshel->cmd[cmd_index]->error_file = \
		ft_strdup(mshel->cmd[cmd_index]->redirect.in_file[i]);
		return (0);
	}
	if (!mshel->cmd[cmd_index]->redirect.input[i + 1])
	{
		if (dup2(fd, STDIN_FILENO) == 1)
			perror("minishell:");
	}
	return (1);
}

int	redirect_input(t_mshel *mshel, int cmd_index, int status)
{
	int	fd;
	int	i;

	(void) status;
	i = 0;
	while (mshel->cmd[cmd_index]->redirect.input[i])
	{
		if ((!mshel->cmd[cmd_index]->redirect.in_file[i]
				|| ft_strchr(mshel->cmd[cmd_index]->redirect.in_file[i], ' '))
			&& mshel->cmd[cmd_index]->redirect.input_expanded[i] == 1)
		{
			mshel->cmd[cmd_index]->error = -3;
			return (0);
		}
		fd = open(mshel->cmd[cmd_index]->redirect.in_file[i], O_RDONLY);
		if (!check_error_n_red(fd, cmd_index, mshel, i))
			return (0);
		close(fd);
		i++;
	}
	return (1);
}

int	open_files(t_mshel *mshel, int cmd_index, int i, int *tmp_fd)
{
	if (!ft_strcmp(mshel->cmd[cmd_index]->redirect.output[i], ">>"))
	{
		(*tmp_fd) = open(mshel->cmd[cmd_index]->redirect.out_file[i], \
		O_RDWR | O_CREAT | O_APPEND, 0644);
		if ((*tmp_fd) == -1)
		{
			mshel->cmd[cmd_index]->error = errno;
			mshel->cmd[cmd_index]->error_file = \
			ft_strdup(mshel->cmd[cmd_index]->redirect.out_file[i]);
			return (0);
		}
	}
	else
	{
		(*tmp_fd) = open(mshel->cmd[cmd_index]->redirect.out_file[i], \
		O_RDWR | O_CREAT | O_TRUNC, 0644);
		if ((*tmp_fd) == -1)
		{
			mshel->cmd[cmd_index]->error = errno;
			mshel->cmd[cmd_index]->error_file = \
			ft_strdup(mshel->cmd[cmd_index]->redirect.out_file[i]);
			return (0);
		}
	}
	return (1);
}

int	redirect_output(t_mshel *mshel, int cmd_index)
{
	int	tmp_fd;
	int	i;

	i = 0;
	while (mshel->cmd[cmd_index]->redirect.output[i])
	{
		if ((!mshel->cmd[cmd_index]->redirect.out_file[i]
				|| ft_strchr(mshel->cmd[cmd_index]->redirect.out_file[i],
					' '))
			&& mshel->cmd[cmd_index]->redirect.output_expanded[i] == 1)
		{
			mshel->cmd[cmd_index]->error = -3;
			return (0);
		}
		if (!open_files(mshel, cmd_index, i, &tmp_fd))
			return (0);
		if (!mshel->cmd[cmd_index]->redirect.output[i + 1])
		{
			if (dup2(tmp_fd, STDOUT_FILENO) == -1)
				perror("minishell:");
		}
		close(tmp_fd);
		i++;
	}
	return (1);
}
