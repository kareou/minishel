/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 11:22:57 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/11 17:18:33 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_utils(char *a)
{
	int	i;

	i = 0;
	if (a[i] == '-' || a[i] == '+')
		i++;
	while (a[i])
	{
		if (!ft_isdigit(a[i]))
			return (0);
		i++;
	}
	return (1);
}

int	exit_function(char **a, t_mshel *shel)
{
	int		i;
	char	*tmp;

	i = 0;
	if (!a[0])
		exit(shel->exit_status);
	else
	{
		if (!exit_utils(a[i]))
		{
			tmp = ft_strjoin("minishell: exit: ", a[i]);
			print_errors(ft_strjoin(tmp, ": numeric argument required"));
			if (shel->cmd_number == 1)
				return (255);
			else
				exit(255);
		}
		i = 0;
		while (a[i])
		{
			if (i > 0)
			{
				print_errors("minishell: exit: too many arguments");
				if (shel->cmd_number == 1)
					return (1);
				else
					exit(1);
			}
			i++;
		}
		i = 0;
		while (a[i])
		{
			if (ft_isdigit(a[i][0]) || (a[i][0] == '-' && ft_isdigit(a[i][1])))
				exit(ft_atoi(a[i]) % 256);
			i++;
		}
	}
	return (0);
}

char	**join_arrays(t_mshel *shel, int index, char *cmd)
{
	int		i;
	int		j;
	char	**new_cmd;

	i = 0;
	while (shel->cmd[index]->args[i] != NULL)
		i++;
	new_cmd = malloc(sizeof(char *) * (i + 2));
	if (shel->cmd[index]->args == NULL)
	{
		new_cmd[0] = ft_strdup(cmd);
		new_cmd[1] = NULL;
		return (new_cmd);
	}
	i = 0;
	new_cmd[0] = ft_strdup(cmd);
	j = 1;
	while (shel->cmd[index]->args[i] != NULL)
	{
		if (!shel->cmd[index]->args[i] || !shel->cmd[index]->args[i][0])
			i++;
		else if (shel->cmd[index]->args[i])
		{
			new_cmd[j++] = ft_strdup(shel->cmd[index]->args[i]);
			i++;
		}
	}
	new_cmd[j] = NULL;
	return (new_cmd);
}

void	run_cmd(t_mshel *shel, int cmd_index, char *cmd)
{
	int	status;
	int	exited;

	exited = 0;
	if (cmd[0] && (!strncmp(cmd, "echo", strlen("echo")) || \
	!ft_strncmp(cmd, "/bin/echo", ft_strlen("/bin/echo"))))
		ech_o(shel, cmd_index);
	else if (cmd[0] && (!strncmp(cmd, "pwd", strlen("pwd")) || \
	!ft_strncmp(cmd, "/bin/pwd", ft_strlen("/bin/pwd"))))
		p_w_d();
	else if (cmd[0] && (!strncmp(cmd, "cd", strlen("cd")) || \
	!ft_strncmp(cmd, "/usr/bin/cd", ft_strlen("/usr/bin/cd"))))
		exited = c_d(shel, shel->cmd[cmd_index]->args[0]);
	else if (cmd[0] && !strncmp(cmd, "exit", strlen("exit")))
		exited = exit_function(shel->cmd[cmd_index]->args, shel);
	else if (cmd[0] && !ft_strncmp(cmd, "export", strlen("export")))
		exited = ft_export(shel, cmd_index);
	else if (cmd[0] && !strncmp(cmd, "unset", strlen("unset")))
		ft_unset(shel, cmd_index);
	else if (cmd[0] && (!strncmp(cmd, "env", strlen("env")) || \
	!ft_strncmp(cmd, "/usr/bin/env", ft_strlen("/usr/bin/env"))))
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

void	execute_cmd(t_mshel *shel, int (*pipe)[2], int cmd_index, int status)
{
	char	*cmd;
	int		red_status;

	cmd = shel->cmd[cmd_index]->cmd;
	red_status = check_redirect_place(shel->cmd[cmd_index]->redirect.in, shel->cmd[cmd_index]->redirect.out);
	if (!redirect_to_pipe(shel, pipe, cmd_index, red_status, status))
	{
		if (!status && shel->cmd[cmd_index]->redirect.heredoc.heredoc_number == 0)
		{
			if (shel->cmd[cmd_index]->error == -3)
				print_errors("minishell: ambiguous redirect");
			else
				error_to_print(shel->cmd[cmd_index]->error, shel->cmd[cmd_index]->error_file);
			shel->exit_status = 1;
			return ;
		}
		else if (status)
			exit(1);
	}
	if (red_status == 1)
	{
		if (!redirect_input(shel, cmd_index, 0))
		{
			if (shel->cmd[cmd_index]->error == -3)
				print_errors("minishell: ambiguous redirect");
			else
				error_to_print(shel->cmd[cmd_index]->error, shel->cmd[cmd_index]->error_file);
			shel->exit_status = 1;
			if (status)
				exit(1);
			else
				return ;
		}
		if (!redirect_output(shel, cmd_index))
		{
			if (shel->cmd[cmd_index]->error == -3)
				print_errors("minishell: ambiguous redirect");
			else
				error_to_print(shel->cmd[cmd_index]->error, shel->cmd[cmd_index]->error_file);
			shel->exit_status = 1;
			if (status)
				exit(1);
			else
				return ;
		}
	}
	if (red_status != 1 && status != 0)
		close_all_pipes(pipe, shel->cmd_number);
	if (shel->cmd[cmd_index]->cmd)
	{
		if (shel->cmd[cmd_index]->redirect.heredoc.heredoc_number == 0)
			run_cmd(shel, cmd_index, cmd);
	}
	if (shel->cmd[cmd_index]->redirect.heredoc.heredoc_number > 0)
		ft_heredoc(cmd_index, shel);
	if (shel->cmd_number > 1)
		exit(0);
	if (shel->cmd[cmd_index]->error != -1)
	{
		if (shel->cmd[cmd_index]->error == -3)
			print_errors("minishell: ambiguous redirect");
		else
			error_to_print(shel->cmd[cmd_index]->error, shel->cmd[cmd_index]->error_file);
		shel->exit_status = 1;
		if (status)
			exit(1);
		else
			return ;
	}
	if (shel->cmd[cmd_index]->redirect.old_output != 0)
	{
		if (dup2(shel->cmd[cmd_index]->redirect.old_output, STDOUT_FILENO) == -1)
			perror("minishell :");
	}
	if (shel->cmd[cmd_index]->redirect.old_input != 0)
	{
		if (dup2(shel->cmd[cmd_index]->redirect.old_input, STDIN_FILENO) == -1)
			perror("minishell :");
	}
}
