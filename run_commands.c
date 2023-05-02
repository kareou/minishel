/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 11:22:57 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/02 16:45:43 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **join_arrays(t_mshel *shel, int index, char *cmd)
{
    char **new_cmd;
    int i;

    i = 0;
    while (shel->cmd[index]->args[i] != NULL)
        i++;
    new_cmd = malloc(sizeof(char *) * (i + 2)); // Parentheses added here
    if (shel->cmd[index]->args == NULL)
    {
        new_cmd[0] = ft_strdup(cmd);
        new_cmd[1] = NULL;
        return (new_cmd);
    }
    i = 0;
    new_cmd[0] = ft_strdup(cmd);
    while (shel->cmd[index]->args[i] != NULL)
    {
        new_cmd[i + 1] = ft_strdup(shel->cmd[index]->args[i]);
        i++;
    }
    new_cmd[i + 1] = NULL; // Add terminating NULL pointer
    return (new_cmd);
}


void	run_cmd(t_mshel *shel , int cmd_index, char *cmd)
{
	int	status;
	int	exited;

	exited = 0;
	if (!strncmp(cmd, "echo", strlen(cmd)) || !ft_strncmp(cmd, "/bin/echo", ft_strlen(cmd)))
		ech_o(shel, cmd_index);
	else if (!strncmp(cmd, "pwd", strlen(cmd)) || !ft_strncmp(cmd, "/bin/pwd", ft_strlen(cmd)))
		p_w_d();
	else if (!strncmp(cmd, "cd", strlen(cmd)) || !ft_strncmp(cmd, "/usr/bin/cd", ft_strlen(cmd)))
		exited = c_d(shel, shel->cmd[cmd_index]->args[0]);
	else if (!strncmp(cmd, "exit", strlen(cmd)))
		exit(0);
	else if (!ft_strncmp(cmd, "export", strlen(cmd)))
		ft_export(shel, cmd_index);
	else if (!strncmp(cmd, "unset", strlen(cmd)))
		ft_unset(shel,  cmd_index);
	else if (!strncmp(cmd, "env", strlen(cmd)) || !ft_strncmp(cmd, "/usr/bin/env", ft_strlen(cmd)))
		print_env(shel, 0);
	else
	{
		if (shel->cmd_number == 1)
		{
			if (fork() == 0)
				execute_shell(join_arrays(shel, cmd_index, cmd), shel);
			else
				wait(&status);
			exited = WEXITSTATUS(status);
		}
		else
			execute_shell(join_arrays(shel, cmd_index, cmd), shel);
	}
	shel->exit_status = exited;
}

void execute_cmd(t_mshel *shel, int (*pipe)[2], int cmd_index, int status)
{
	char *cmd;
	int red_status;

	// if (shel->cmd[cmd_index]->cmd[0] == '$' && shel->cmd[cmd_index]->cmd)
	// {
	// 	if(shel->cmd[cmd_index]->cmd[1] == '?')
	// 		cmd  = ft_itoa(shel->exit_status);
	// 	else
	// 		cmd = ft_getenv(shel, shel->cmd[cmd_index]->cmd + 1);
	// }
	// else if (shel->cmd[cmd_index]->cmd)
		cmd = shel->cmd[cmd_index]->cmd;
	red_status = check_redirect_place(shel->cmd[cmd_index]->redirect.in, shel->cmd[cmd_index]->redirect.out);
	if (!redirect_to_pipe(shel, pipe, cmd_index, red_status, status))
	{
		if (!status && shel->cmd[cmd_index]->redirect.heredoc.heredoc_number == 0)
		{
			print_errors(ft_strjoin("minishel :", strerror(shel->cmd[cmd_index]->error)));
			return;
		}
		else if (status)
			exit(1);
	}
	if(red_status == 1)
	{
		redirect_input(shel, cmd_index, 0);
		redirect_output(shel, cmd_index);
	}
	if (red_status != 1 && status != 0)
		close_all_pipes(pipe, shel->cmd_number);
	if(shel->cmd[cmd_index]->cmd && shel->cmd[cmd_index]->cmd[0])
	{
		if(shel->cmd[cmd_index]->redirect.heredoc.heredoc_number == 0)
			run_cmd(shel,cmd_index, cmd);
		else
			ft_heredoc(cmd_index, shel);
	}
	if(shel->cmd_number > 1)
			exit(0);
	if(shel->cmd[cmd_index]->error != -1)
		print_errors(ft_strjoin("minishel :", strerror(shel->cmd[cmd_index]->error)));
	if (shel->cmd[cmd_index]->redirect.old_output != 0)
	{
		if (dup2(shel->cmd[cmd_index]->redirect.old_output, STDOUT_FILENO) == -1)
			perror("minishel :");
	}
	if (shel->cmd[cmd_index]->redirect.old_input != 0)
		if (dup2(shel->cmd[cmd_index]->redirect.old_input, STDIN_FILENO) == -1)
			perror("minishel :");
}
