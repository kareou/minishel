/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 12:31:38 by mkhairou          #+#    #+#             */
/*   Updated: 2023/04/28 15:31:30 by mkhairou         ###   ########.fr       */
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

void    execute_shell(char *cmd, t_mshel *shel)
{
	char *path;
	char **tmp_path;
	char *p;
	char **arg;
	int i;

	i = 0;
	path = ft_getenv(shel, "PATH");
	arg = mini_split(cmd);

	if(path)
	{
		tmp_path = ft_split(path, 58);
		if (arg[0][0] == '/' || (arg[0][0] == '.' && arg[0][1] == '/'))
		{
			if(execve(arg[0],arg,NULL) == -1)
			{
				print_errors(ft_strjoin("minishell :", strerror(errno)));
				exit(126);
			}
		}
		while(tmp_path[i])
		{
			p = join_cnp(tmp_path[i], arg[0]);
			if(!access(p, F_OK))
				execve(p,arg,NULL);
			free(p);
			i++;
		}
	}
	if(path)
		print_errors(ft_strjoin("minishell : command not found : ", arg[0]));
	else
		print_errors(ft_strjoin("minishell : No such file or directory : ", arg[0]));
	exit(127);
}
