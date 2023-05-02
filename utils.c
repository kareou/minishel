/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 13:25:37 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/01 15:33:09 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_errors(char *a)
{
	write(2, a, ft_strlen(a));
	write(2, "\n", 1);
}

long	checking_overwrite(t_mshel *shel,int cmd_index)
{
	int fd[2];
	char buffer[1000000];
	ssize_t n;

	pipe(fd);
	if (fork() == 0) {
	    close(fd[0]);
	    dup2(fd[1], STDOUT_FILENO);
	    run_cmd(shel, cmd_index, shel->cmd[cmd_index]->cmd);
	    sleep(1);
	    exit(0);
	}
	else
	{
    	close(fd[1]);
    	n = read(fd[0], buffer, 1000000);
	    buffer[n] = '\0';
	}
	int	i = 0;
	while (shel->cmd[cmd_index]->redirect.out_file[i])
	{
		if (!shel->cmd[cmd_index]->redirect.out_file[i + 1])
			break;
		i++;
	}
	int	tmp = open(shel->cmd[cmd_index]->redirect.out_file[i],O_RDONLY);
	ssize_t	size = read(tmp,buffer,1024);
	close(tmp);
	return ((long)n - (long)size);
}
