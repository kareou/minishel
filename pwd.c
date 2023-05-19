/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 20:08:26 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/19 20:08:51 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_pwd(char *old, t_mshel *shel)
{
	char	*update_old;
	char	*update_new;
	char	a[PATH_MAX];

	update_old = ft_strjoin("OLDPWD=", old);
	add_env(shel, update_old);
	add_x_env(shel, update_old);
	free(update_old);
	update_new = ft_strjoin("PWD=", getcwd(a, sizeof(a)));
	add_env(shel, update_new);
	add_x_env(shel, update_new);
	free(update_new);
}

int	c_d(t_mshel *shel, char *a)
{
	int		ret;
	char	*thispath;

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
		a = ft_strjoin("/Users/", ft_getenv(shel, "USER"));
	thispath = ft_getenv(shel, "PWD");
	ret = chdir(a);
	if (ret == -1)
	{
		print_errors(ft_strjoin("minishell: ", strerror(errno)));
		return (1);
	}
	change_pwd(thispath, shel);
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
