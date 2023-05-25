/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 11:28:46 by asekkak           #+#    #+#             */
/*   Updated: 2023/05/25 22:33:16 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_mshel	g_ms;

void	sigint_handler_c(int sig)
{
	(void)sig;
	if (g_ms.id != 0)
		kill(g_ms.id, SIGKILL);
	printf("\n");
	rl_on_new_line();
	// rl_replace_line("", 0);
	g_ms.exit_status = 1;
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

	signal(SIGQUIT, sigint_handler_quit);
	signal(SIGINT, sigint_handler_c);
	dup_env(&g_ms, env);
	g_ms.exit_status = 0;
	while (1)
	{
		g_ms.id = 0;
		input = readline("minishell> ");
		if (input == NULL)
			exiting(&g_ms, g_ms.exit_status, 0);
		add_history(input);
		tmp = ft_strtrim(input, " ");
		if (check_syntax(tmp, -1, 0))
			lexer(fix_pipe(tmp), &g_ms);
		else
			g_ms.exit_status = 2;
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
