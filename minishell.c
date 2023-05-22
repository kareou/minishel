/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 11:28:46 by asekkak           #+#    #+#             */
/*   Updated: 2023/05/22 16:44:59 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler_c(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	// rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_handler_quit(int sig)
{
	(void)sig;
}

char	*fix_pipe(char *a)
{
	int		i;
	int		j;
	int		quote;
	char	*b;

	i = 0;
	j = 0;
	quote = 0;
	b = malloc(sizeof(char) * (ft_strlen(a) * 2));
	while (a[i])
	{
		if (a[i] == '"' || a[i] == '\'')
		{
			if (quote == 0)
				quote = a[i];
			else
				quote = 0;
		}
		if (a[i] == '|' && quote == 0)
		{
			b[j++] = ' ';
			b[j++] = '|';
			b[j++] = ' ';
		}
		else
			b[j++] = a[i];
		i++;
	}
	b[j] = '\0';
	return (b);
}

void	minishell(char **env)
{
	char	*input;
	char	*tmp;
	t_mshel	*shel;

	signal(SIGQUIT, sigint_handler_quit);
	signal(SIGINT, sigint_handler_c);
	shel = malloc(sizeof(t_mshel));
	dup_env(shel, env);
	shel->exit_status = 0;
	shel->exapnd_herdoc = malloc(sizeof(int) * 10);
	shel->status = malloc(sizeof(int) * 10000);
	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL)
			exiting(shel, shel->exit_status, 0);
		add_history(input);
		tmp = ft_strtrim(input, " ");
		if (check_syntax(tmp, -1, 0))
			lexer(fix_pipe(tmp), shel);
		else
			shel->exit_status = 2;
		free(tmp);
		free(input);
	}
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	minishell(env);
}
