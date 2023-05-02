/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 12:31:38 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/01 16:45:39 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**mini_split(char *a)
{
	int i;
	char **val;

	i = 0;
	while (a[i] && a[i] != ' ')
		i++;
	if (!ft_strncmp(ft_substr(a, 0, i), "awk", i))
	{
		val = malloc(sizeof(char *) * 3);
		while (a[i] && a[i] != ' ')
			i++;
		val[0] = ft_substr(a, 0, i);
		if(a[i])
		{
			val[1] = ft_substr(a, i + 1, ft_strlen(a));
			val[2] = NULL;
		}
		else
			val[1] = NULL;
	}
	else
		val = ft_split(a, ' ');
	return(val);
}

int c_d(t_mshel *shel, char *a)
{
	int ret;

	if (!a || !ft_strncmp(a, "~", 1))
	{
		free(a);
		a = ft_strjoin("/Users/", ft_getenv(shel, "USER"));
	}
	ret = chdir(a);
	if (ret == -1)
	{
		printf("cd : %s : %s\n", strerror(errno), a);
		free(a);
		return (1);
	}
	free(a);
	return(0);
}

void p_w_d(void)
{
	char a[PATH_MAX];

	getcwd(a, sizeof(a));
	printf("%s\n", a);
}

char    *join_cnp(char *a, char *b)
{
	char *ret;
	char *p;

	ret = ft_strjoin(a,"/");
	p = ret;
	ret = ft_strjoin(ret , b);
	free(p);
	return(ret);
}

void    execute_shell(char **cmd, t_mshel *shel)
{
	char *path;
	char **tmp_path;
	char *p;
	int i;

	i = 0;
	path = ft_getenv(shel, "PATH");
	if(path)
	{
		tmp_path = ft_split(path, 58);
		if (cmd[0][0] == '/' || (cmd[0][0] == '.' && cmd[0][1] == '/'))
		{
			if(execve(cmd[0],cmd,NULL) == -1)
			{
				print_errors(ft_strjoin("minishell :", strerror(errno)));
				exit(126);
			}
		}
		while(tmp_path[i])
		{
			p = join_cnp(tmp_path[i], cmd[0]);
			if(!access(p, F_OK))
				execve(p,cmd,NULL);
			free(p);
			i++;
		}
	}
	if(path)
		print_errors(ft_strjoin("minishell : command not found : ", cmd[0]));
	else
		print_errors(ft_strjoin("minishell : No such file or directory : ", cmd[0]));
	exit(127);
}
