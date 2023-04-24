/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 11:22:57 by mkhairou          #+#    #+#             */
/*   Updated: 2023/04/24 15:52:04 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>
char *join_arrays(t_mshel *shel, int index, char *cmd)
{
	char	*new_cmd;
	int		i;

	i = 0;
	if (shel->cmd[index]->args == NULL)
	{
		new_cmd = ft_strdup(cmd);
		return (new_cmd);
	}
	new_cmd = ft_strdup(cmd);
	while (shel->cmd[index]->args[i] != NULL)
	{
		new_cmd = ft_strjoin(new_cmd, " ");
		new_cmd = ft_strjoin(new_cmd, shel->cmd[index]->args[i]);
		i++;
	}
	return (new_cmd);
}

void	run_cmd(t_mshel *shel , int cmd_index, char *cmd)
{
	int	tmp;
	if (!strncmp(cmd, "echo", strlen(cmd)) || !ft_strncmp(cmd, "/bin/echo", ft_strlen(cmd)))
		ech_o(shel, cmd_index);
	else if (!strncmp(cmd, "pwd", strlen(cmd)) || !ft_strncmp(cmd, "/bin/pwd", ft_strlen(cmd)))
		p_w_d();
	else if (!strncmp(cmd, "cd", strlen(cmd)) || !ft_strncmp(cmd, "/usr/bin/cd", ft_strlen(cmd)))
	{
		c_d(shel, shel->cmd[cmd_index]->args[0]);
	}
	else if (!strncmp(cmd, "exit", strlen(cmd)))
		exit(0);
	else if (!ft_strncmp(cmd, "export", strlen(cmd)))
		ft_export(shel, cmd_index);
	else if (!strncmp(cmd, "unset", strlen(cmd)))
		ft_unset(shel, shel->cmd[cmd_index]->args[0]);
	else if (!strncmp(cmd, "env", strlen(cmd)) || !ft_strncmp(cmd, "/usr/bin/env", ft_strlen(cmd)))
		print_env(shel, 0);
	else
	{
		if (shel->cmd_number == 1)
		{
			tmp = fork();
			if (tmp == 0)
				execute_shell(join_arrays(shel, cmd_index, cmd), shel);
			else
				waitpid(tmp, NULL, 0);
		}
		else
		{
			char *o = join_arrays(shel, cmd_index, cmd);
			execute_shell(o, shel);
		}
	}
}

void execute_cmd(t_mshel *shel, int (*pipe)[2], int cmd_index, int status)
{
	char *cmd;
	int red_status;

	if (shel->cmd[cmd_index]->cmd[0] == '$' && shel->cmd[cmd_index]->cmd)
		cmd = ft_getenv(shel, shel->cmd[cmd_index]->cmd + 1);
	if (shel->cmd[cmd_index]->cmd)
		cmd = shel->cmd[cmd_index]->cmd;
	red_status = check_redirect_place(shel->cmd[cmd_index]->redirect.in, shel->cmd[cmd_index]->redirect.out);
	if (red_status != 1)
	{
		if (!redirect_to_pipe(shel, pipe, cmd_index, red_status, status))
		{
			if (!status)
				return;
			else
				exit(1);
		}
	}
	if(red_status == 1)
	{
		redirect_input(shel, cmd_index, 0);
		redirect_output(shel, cmd_index);
	}
	if (red_status != 1 && status != 0)
		close_all_pipes(pipe, shel->cmd_number);
	if(shel->cmd[cmd_index]->cmd)
	{
		if(shel->cmd[cmd_index]->redirect.heredoc.heredoc_number == 0)
			run_cmd(shel,cmd_index, cmd);
		else
			ft_heredoc(cmd, shel->cmd[cmd_index]->args[0],cmd_index, shel);
	}
	if(shel->cmd_number > 1)
	{
		if((cmd_index + 1 == shel->cmd_number) && !strcmp(cmd, "cd"))
			exit(0);
		if(strcmp(cmd, "cd"))
			exit(0);
	}
	if (shel->cmd[cmd_index]->redirect.old_output != 0)
		dup2(shel->cmd[cmd_index]->redirect.old_output, STDOUT_FILENO);
	if (shel->cmd[cmd_index]->redirect.old_input != 0)
		dup2(shel->cmd[cmd_index]->redirect.old_input, STDIN_FILENO);
}
