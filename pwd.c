/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 20:08:26 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/25 22:53:33 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_pwd(char *old, t_mshel *shel)
{
	char	*update_old;
	char	*update_new;
	char	a[PATH_MAX];

	update_old = ft_strjoin("OLDPWD=", old);
	add_env(shel, update_old, -1);
	add_x_env(shel, update_old);
	free(update_old);
	update_new = ft_strjoin("PWD=", getcwd(a, sizeof(a)));
	add_env(shel, update_new, -1);
	add_x_env(shel, update_new);
	free(update_new);
}

int	test(char *a, char *thispath, t_mshel *shel, int i)
{
	int		ret;
	char	*error;

	ret = chdir(a);
	if (ret == -1)
	{
		error = ft_strjoin("minishell: ", strerror(errno));
		print_errors(error);
		free(error);
		return (1);
	}
	if (i)
		free(a);
	change_pwd(thispath, shel);
	return (0);
}

void	handel_telda(t_mshel *shel, char **a, int *i)
{
	char	*tmp;

	tmp = ft_strjoin("/Users/", ft_getenv(shel, "USER"));
	if ((*a)[1])
		(*a) = smart_join(tmp, &(*a)[1]);
	else
		(*a) = tmp;
	(*i) = 1;
}

int	c_d(t_mshel *shel, char *a)
{
	char	*thispath;
	int		i;

	i = 0;
	if (!a)
	{
		a = ft_getenv(shel, "HOME");
		if (!a)
		{
			print_errors("minishell: cd: HOME not set");
			return (1);
		}
	}
	if (!ft_strncmp(a, "~", 1))
		handel_telda(shel, &a, &i);
	thispath = ft_getenv(shel, "PWD");
	if (test(a, thispath, shel, i))
		return (1);
	return (0);
}

void	p_w_d(void)
{
	char	a[PATH_MAX];
	int		i;

	getcwd(a, sizeof(a));
	i = 0;
	while (a[i])
	{
		write(1, &a[i], 1);
		i++;
	}
	write(1, "\n", 1);
}
