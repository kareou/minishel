/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 12:31:38 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/25 22:20:50 by mkhairou         ###   ########.fr       */
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
	char	*tmp;
	int		i;

	i = 0;
	tmp = fix_path(path, 0, 0);
	tmp_path = ft_split(tmp, 58);
	free(tmp);
	while (tmp_path[i])
	{
		p = join_cnp(tmp_path[i], cmd[0]);
		if (!access(p, F_OK))
		{
			if (execve(p, cmd, shel->env) == -1)
			{
				free_path(tmp_path, i + 1);
				perror("minishell");
			}
		}
		free(p);
		i++;
	}
}

void	errors(char **cmd, char *path, t_mshel *shel)
{
	char	*tmp;

	if (path && cmd[0][0])
		run_it(path, cmd, shel);
	tmp = ft_strjoin("minishell: ", cmd[0]);
	if (path)
	{
		tmp = smart_join(tmp, ": command not found");
		print_errors(tmp);
	}
	else
	{
		tmp = smart_join(tmp, ": No such file or directory");
		print_errors(tmp);
	}
	free(tmp);
	exit(127);
}

void	execute_shell(char **cmd, t_mshel *shel)
{
	char		*path;
	struct stat	fileinf;

	path = ft_getenv(shel, "PATH");
	if (cmd[0][0] == '/' || (cmd[0][0] == '.'))
	{
		if (execve(cmd[0], cmd, shel->env) == -1)
		{
			if (stat(cmd[0], &fileinf) == 0)
			{
				if (S_ISDIR(fileinf.st_mode))
					print_errors("minishell: is a directory");
				else
					print_errors("minishell: Not a directory");
				exit(126);
			}
			else
			{
				perror("minishell");
				exit(127);
			}
		}
	}
	errors(cmd, path, shel);
}
