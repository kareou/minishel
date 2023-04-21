/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 12:31:38 by mkhairou          #+#    #+#             */
/*   Updated: 2023/04/20 19:59:27 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void c_d(t_mshel *shel, char *a)
{
	int ret;

	// printf("here\n");
	if (!a[0] || !ft_strncmp(a, "~", 1))
	{
		free(a);
		a = ft_strjoin("/Users/", ft_getenv(shel, "USER"));
	}
	ret = chdir(a);
	if (ret == -1)
	{
		printf("cd : %s : %s\n", strerror(errno), a);
	}
	free(a);
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
	arg = ft_split(cmd, ' ');
	if(path)
	{
		tmp_path = ft_split(path, 58);
		if (arg[0][0] == '/' || (arg[0][0] == '.' && arg[0][1] == '/'))
			execve(arg[0],arg,NULL);
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
		printf("minishell : command not found : %s\n", arg[0]);
	else
		printf("minishell : No such file or directory : %s\n", arg[0]);
	exit(127);
}
