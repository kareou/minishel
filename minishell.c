/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 11:28:46 by asekkak           #+#    #+#             */
/*   Updated: 2023/04/29 14:12:15 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_sringcpy(char **str, char *content)
{
    int i = 0;

    while (content[i])
    {
        (*str)[i] = content[i];
        i++;
    }

    (*str)[i] = '\0';
}

t_lexer *add_node(char **content, int start, int end)
{
    t_lexer *lexer = malloc(sizeof(t_lexer));
    lexer->str = malloc(sizeof(char *) * (end - start + 1));
    if (!lexer)
        return (0);

    for (int i = start; i < end; i++)
    {
        lexer->str[i - start] = malloc(sizeof(char) * (strlen(content[i]) + 1));
        ft_sringcpy(&(lexer->str[i - start]), content[i]);
    }

    lexer->str[end - start] = NULL;
    lexer->next = NULL;
    return (lexer);
}

void ft_add_back(t_lexer **lst, t_lexer *new)
{
	t_lexer	*lastnode;

	lastnode = *lst;
	if (*lst)
	{
		while (lastnode->next)
			lastnode = lastnode->next;
		lastnode->next = new;
	}
	else
		*lst = new;
}

void lexer(char *input, t_mshel *shel)
{
	int i;
	int j;

 	i = 0;
	j = 0;
  	t_lexer *newNode;
	t_lexer *lst = NULL;
	char **string = calloc(1024, 1024);
 	string = ft_split(input, ' ');
	while(string[i])
	{
 		if(ft_strncmp(string[i],"|",2) == 0)
		{
 			newNode = add_node(string, j, i + 1);
  			ft_add_back(&lst, newNode);
			j = i + 1;
   		}
		i++;
	}
 	if(i - j >= 1)
	{
		newNode = add_node(string, j, i);
 		ft_add_back(&lst, newNode);
	}
	parser(lst, shel);
}

void sigint_handler_c(int sig) {
	 	(void)sig;
 	printf("\n");
    rl_on_new_line();
    // rl_replace_line("", 0);
    rl_redisplay();
}

void sigint_handler_quit(int sig) {
	(void)sig;
}

void minishell(char **env)
{
	char *input;
	t_mshel *shel;

    signal(SIGQUIT,sigint_handler_quit);
    signal(SIGINT,sigint_handler_c);
	shel = malloc(sizeof(t_mshel));
	dup_env(shel, env);
	shel->exit_status = 0;
  	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL)
		{
             printf("exit\n");
             break;
        }
		// if (ft_strncmp(input, "exit",5) == 0) {
        //     printf("Exiting...\n");
        //     break;
        // }
 		add_history(input);
		lexer(input, shel);
		// free(input);
 	}
}

int main(int ac, char **av, char **env) {

	(void)ac;
	(void)av;
     minishell(env);
}
