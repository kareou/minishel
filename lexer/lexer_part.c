/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_part.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 13:05:10 by asekkak           #+#    #+#             */
/*   Updated: 2023/05/23 12:11:59 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

char	**alloc_lexer_string(char *input)
{
	char	**string;
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '|' && input[i])
			count++;
		i++;
	}
	string = malloc(sizeof(char *) * (count + 1));
	if (!string)
		return (0);
	return (string);
}

void	lexer_part_separate(char **string, t_mshel *shel, t_lexer *newnode,
		t_lexer *lst)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (string[i])
	{
		if (ft_strncmp(string[i], "|", 2) == 0 && shel->status[i] == 0)
		{
			newnode = add_node(string, j, i + 1);
			ft_add_back(&lst, newnode);
			j = i + 1;
		}
		i++;
	}
	if (i - j >= 1)
	{
		newnode = add_node(string, j, i);
		ft_add_back(&lst, newnode);
	}
	free_array(string);
	parser(lst, shel, shel->store_input);
}

void	lexer(char *input, t_mshel *shel)
{
	char		**string;
	t_lexer		*newnode;
	t_lexer		*lst;
	t_indexs	index;

	index.i = 0;
	index.j = 0;
	lst = NULL;
	newnode = NULL;
	shel->exapnd_herdoc = malloc(sizeof(int) * word_num(input, '<'));
	shel->status = ft_calloc(ft_strlen(input), sizeof(int));
	string = better_parsing(input, shel, &index);
	free(input);
	lexer_part_separate(string, shel, newnode, lst);
}
