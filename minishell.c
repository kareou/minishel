/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 11:28:46 by asekkak           #+#    #+#             */
/*   Updated: 2023/05/19 14:46:29 by mkhairou         ###   ########.fr       */
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
			exit(shel->exit_status);
		add_history(input);
		tmp = ft_strtrim(input, " ");
		if (check_syntax(tmp))
			lexer(input, shel);
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
