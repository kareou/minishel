/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 12:31:38 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/19 20:08:50 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_path(char **path, int i)
{
	while (path[i])
	{
		free(path[i++]);
	}
}

char	*join_cnp(char *a, char *b)
{
	char	*ret;
	char	*p;

	ret = ft_strjoin(a, "/");
	free(a);
	p = ret;
	ret = ft_strjoin(ret, b);
	free(p);
	return (ret);
}

void	run_it(char *path, char **cmd, t_mshel *shel)
{
	char	**tmp_path;
	char	*p;
	int		i;

	i = 0;
	tmp_path = ft_split(path, 58);
	while (tmp_path[i])
	{
		p = join_cnp(tmp_path[i], cmd[0]);
		if (!access(p, F_OK))
			execve(p, cmd, shel->env);
		free(p);
		i++;
	}
}

void	execute_shell(char **cmd, t_mshel *shel)
{
	char	*path;
	int		i;

	i = 0;
	path = ft_getenv(shel, "PATH");
	if (cmd[0][0] == '/' || (cmd[0][0] == '.'))
	{
		if (execve(cmd[0], cmd, shel->env) == -1)
		{
			print_errors(ft_strjoin("minishell: ", strerror(errno)));
			exit(127);
		}
	}
	if (path)
		run_it(path, cmd, shel);
	if (path)
		print_errors("minishell: command not found");
	else
		print_errors("minishell: No such file or directory");
	exit(127);
}
