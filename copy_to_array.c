/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_to_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 10:46:32 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/13 21:09:21 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	input_count(char **a, int state)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (a[i])
	{
		if((!strcmp(a[i], "<") || !strcmp(a[i], "<<")) && state == 1)
			j++;
		if((!strcmp(a[i], ">") || !strcmp(a[i], ">")) && state == 0)
			j++;
		i++;
	}
	return(0);
}

int	find_cmd(char **a, t_mshel *shel)
{
	int	i;

	(void)shel;
	i = 0;
	if (!a[0])
		return (-2);
	while (a[i])
	{
		if ((a[i] && (!strcmp(a[i], ">") || !strcmp(a[i], "<") || !strcmp(a[i],
						">>") || !strcmp(a[i], "<<"))) && shel->status[i] == 0)
		{
			i += (a[i + 1] != NULL) ? 2 : 1;
		}
		else if (a[i] && a[i][0] && !strcmp(a[i], "|") && i != 0)
			i++;
		else if (a[i])
			return (i);
	}
	return (-1);
}

void	transfer_input(t_lexer *lexer, t_mshel *shel, int *start, int k, int *r)
{
	shel->cmd[k]->redirect.in = 1;
	shel->cmd[k]->redirect.input[(*r)] = ft_strdup(lexer->str[(*start)]);
	if (lexer->str[(*start) + 1] && !lexer->str[(*start) + 1][0])
	{
		shel->cmd[k]->redirect.in_file[(*r)] = NULL;
		shel->cmd[k]->redirect.input_expanded[(*r)] = shel->status[(*start)
			+ 1];
		(*start) += 1;
	}
	else if (lexer->str[(*start) + 1])
	{
		shel->cmd[k]->redirect.in_file[(*r)] = ft_strdup(lexer->str[(*start)
				+ 1]);
		shel->cmd[k]->redirect.input_expanded[(*r)] = shel->status[(*start)
			+ 1];
		(*start) += 2;
	}
	else
	{
		shel->cmd[k]->redirect.in_file[(*r)] = ft_calloc(1, 1);
		shel->cmd[k]->redirect.input_expanded[(*r)] = shel->status[(*start)
			+ 1];
		(*start)++;
	}
	(*r)++;
}

void	transfer_output(t_lexer *lexer, t_mshel *shel, int *start, int k, int *o)
{
	shel->cmd[k]->redirect.out = 1;
	shel->cmd[k]->redirect.output[(*o)] = ft_strdup(lexer->str[(*start)]);
	if (lexer->str[(*start) + 1] && !lexer->str[(*start) + 1][0])
	{
		shel->cmd[k]->redirect.out_file[(*o)] = NULL;
		shel->cmd[k]->redirect.output_expanded[(*o)] = shel->status[(*start)
			+ 1];
		(*start) += 1;
	}
	else if (lexer->str[(*start) + 1])
	{
		shel->cmd[k]->redirect.out_file[(*o)] = ft_strdup(lexer->str[(*start)
				+ 1]);
		shel->cmd[k]->redirect.output_expanded[(*o)] = shel->status[(*start)
			+ 1];
		(*start) += 2;
	}
	else
	{
		shel->cmd[k]->redirect.out_file[(*o)] = ft_calloc(1, 1);
		shel->cmd[k]->redirect.output_expanded[(*o)] = shel->status[(*start)
			+ 1];
		(*start)++;
	}
	(*o)++;
}

void	transfer_redirection(t_lexer *lexer, t_mshel *shel, int *start, int k, int *r, int *o)
{
	if (!strcmp(lexer->str[(*start)], "<"))
		transfer_input(lexer, shel, start, k, r);
	if (lexer->str[(*start)] && !strcmp(lexer->str[(*start)], "<<"))
	{
		shel->cmd[k]->redirect.heredoc.heredoc_number++;
		(*start)++;
	}
	if (lexer->str[(*start)] && (!strcmp(lexer->str[(*start)], ">")
			|| !strcmp(lexer->str[(*start)], ">>")))
		transfer_output(lexer, shel, start, k, o);
}

void	transfer_part_2(t_lexer *lexer, t_mshel *shel , int *start, int k, int *r, int *o, int *i)
{
	if ((!strcmp(lexer->str[(*start)], ">") || !strcmp(lexer->str[(*start)], \
	"<") || !strcmp(lexer->str[(*start)], ">>") || \
	!strcmp(lexer->str[(*start)], "<<")) && shel->status[(*start)] == 0)
		transfer_redirection(lexer, shel, start, k, r, o);
	else
	{
		if (lexer->str[(*start)])
		{
			if (strcmp(lexer->str[(*start)], "<<") && (*start) > 0 && \
			shel->cmd[k]->redirect.heredoc.heredoc_number > 0
				&& !strcmp(lexer->str[(*start) - 1], "<<"))
			{
				shel->cmd[k]->redirect.heredoc.delemiter[shel->cmd[k]->\
				redirect.heredoc.heredoc_number - 1] \
				= ft_strdup(lexer->str[(*start)]);
			}
			else
			{
				shel->cmd[k]->args[(*i)] = ft_strdup(lexer->str[(*start)]);
				(*i)++;
			}
			(*start)++;
		}
	}
}

void	transfer(t_lexer *lexer, t_mshel *shel, int j, int k, int flag)
{
	int	i;
	int	r;
	int	o;
	int	start;

	start = 0;
	i = 0;
	r = 0;
	o = 0;
	while (lexer->str[start])
	{
		if (start == j)
		{
			start++;
			if (flag)
				start++;
		}
		if (lexer->str[start])
		{
			if (strcmp(lexer->str[start], "|") == 0 && start != 0
				&& shel->status[start] == 0)
				start++;
		}
		if (lexer->str[start])
			transfer_part_2(lexer, shel, &start, k, &r, &o, &i);
	}
	shel->cmd[k]->args[i] = NULL;
	if (lexer->table->redire > 0)
	{
		shel->cmd[k]->redirect.input[r] = NULL;
		shel->cmd[k]->redirect.in_file[r] = NULL;
		shel->cmd[k]->redirect.output[o] = NULL;
		shel->cmd[k]->redirect.out_file[o] = NULL;
	}
}

void	alloc_redirect(t_mshel *shel, int i, char **a)
{
	(void)a;
	shel->cmd[i]->redirect.output = malloc(sizeof(char *) * 10);
	shel->cmd[i]->redirect.out_file = malloc(sizeof(char *) * 10);
	shel->cmd[i]->redirect.input_expanded = malloc(sizeof(char *) * 10);
	shel->cmd[i]->redirect.output_expanded = malloc(sizeof(char *)
			* 10);
	shel->cmd[i]->redirect.input = malloc(sizeof(char *) * 10);
	shel->cmd[i]->redirect.in_file = malloc(sizeof(char *) * 10);
	shel->cmd[i]->redirect.heredoc.cmd = ft_calloc(1, 1);
	shel->cmd[i]->redirect.heredoc.delemiter = malloc(sizeof(char *)
			* 10);
}

void	transfer_herdoc_args(t_mshel *shel, int i, int cmd_position, t_lexer *head)
{
	if (shel->cmd[i]->cmd)
		shel->cmd[i]->redirect.heredoc.cmd = \
		ft_strdup(shel->cmd[i]->cmd);
	if (head->str[cmd_position + 1] && \
	strcmp(head->str[cmd_position + 1], "<<"))
		shel->cmd[i]->redirect.heredoc.cmd = ft_strjoin(\
		shel->cmd[i]->redirect.heredoc.cmd, \
		ft_strjoin(" ", head->str[cmd_position + 1]));
}

void	transfer_cmd(t_lexer *lexer, t_mshel *shel)
{
	int		i;
	int		j;
	int		cmd_position;
	t_lexer	*head;

	i = 0;
	head = lexer;
	while (head)
	{
		j = 0;
		shel->cmd[i]->redirect.heredoc.heredoc_number = 0;
		cmd_position = find_cmd(head->str, shel);
		if (cmd_position == -1)
			shel->cmd[i]->cmd = NULL;
		else
			shel->cmd[i]->cmd = ft_strdup(head->str[cmd_position]);
		shel->cmd[i]->redirect.in = 0;
		shel->cmd[i]->redirect.out = 0;
		if (head->flag != 0)
			shel->cmd[i]->flags = 1;
		if (head->table->redire > 0)
			alloc_redirect(shel, i, head->str);
		if (head->flag != 0)
			transfer(head, shel, cmd_position, i, 1);
		else
			transfer(head, shel, cmd_position, i, 0);
		if (shel->cmd[i]->redirect.heredoc.heredoc_number > 0)
			transfer_herdoc_args(shel, i, cmd_position, head);
		i++;
		head = head->next;
	}
	shel->cmd_number = i;
}

void	dup_env(t_mshel *shel, char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	shel->env = malloc(sizeof(char *) * (i + 1));
	shel->x_env = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env[i])
	{
		shel->x_env[i] = ft_strdup(env[i]);
		shel->env[i] = ft_strdup(env[i]);
		i++;
	}
	shel->x_env[i] = NULL;
	shel->env[i] = NULL;
}

void	transfer_to_array(t_lexer *lexer, int size_arrays, t_mshel *mshel)
{
	int	pipe[0][2];
	int	i;

	mshel->cmd = malloc(sizeof(t_cmd *) * size_arrays);
	if (!mshel->cmd)
	{
		printf("Failed to allocate memory for mshel->cmd\n");
		exit(1);
	}
	i = 0;
	while (i < size_arrays)
	{
		mshel->cmd[i] = malloc(sizeof(t_cmd));
		mshel->cmd[i]->args = malloc(sizeof(char *) * 101);
		mshel->cmd[i]->error = -1;
		mshel->cmd[i]->redirect.old_output = 0;
		mshel->cmd[i]->redirect.old_input = 0;
		mshel->cmd[i]->redirect.ambugius = 0;
		i++;
	}
	transfer_cmd(lexer, mshel);
	free_lexer(lexer);
	if (mshel->cmd_number == 1)
		execute_cmd(mshel, pipe, 0, 0);
	else
		pipe_and_start(mshel);
	free_all(mshel, size_arrays);
}
