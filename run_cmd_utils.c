/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 19:51:08 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/20 21:15:48 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	join_args( t_mshel *shel, int index, char **new_cmd)
{
	int		j;
	int		i;

	j = 1;
	i = 0;
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
}

char	**join_arrays(t_mshel *shel, int index, char *cmd)
{
	int		i;
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
	new_cmd[0] = ft_strdup(cmd);
	join_args(shel, index, new_cmd);
	return (new_cmd);
}

int	run_non_builtin(t_mshel *shel, int cmd_index, char *cmd)
{
	int		status;
	int		exited;
	char	**l;
	int		i;

	exited = 0;
	if (shel->cmd_number == 1)
	{
		l = join_arrays(shel, cmd_index, cmd);
		if (fork() == 0)
			execute_shell(l, shel);
		else
			wait(&status);
		i = 0;
		while (l[i])
		{
			free(l[i++]);
		}
		free(l);
		exited = WEXITSTATUS(status);
	}
	else
		execute_shell(join_arrays(shel, cmd_index, cmd), shel);
	return (exited);
}

void	run_cmd(t_mshel *shel, int cmd_index, char *cmd)
{
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
		exited = ft_export(shel, cmd_index, -1);
	else if (cmd[0] && !strncmp(cmd, "unset", strlen("unset")))
		ft_unset(shel, cmd_index);
	else if (cmd[0] && (!strncmp(cmd, "env", strlen("env")) || \
	!ft_strncmp(cmd, "/usr/bin/env", ft_strlen("/usr/bin/env"))))
		print_env(shel, 0);
	else
		exited = run_non_builtin(shel, cmd_index, cmd);
	shel->exit_status = exited;
}
