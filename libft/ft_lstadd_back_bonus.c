/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 10:34:25 by asekkak           #+#    #+#             */
/*   Updated: 2023/05/19 14:30:00 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*lastnode;

	lastnode = *lst;
	if (*lst)
	{
		while (lastnode->next)
		{
			lastnode = lastnode->next;
		}
		lastnode->next = new;
	}
	else
		*lst = new;
}

// int main()
// {
// 	t_list *lst = NULL;
// 	t_list *node = NULL;

// 	node = ft_lstnew("ahmed");
// 	ft_lstadd_back(&lst,node);
// 	node = ft_lstnew("sekak");
// 	ft_lstadd_back(&lst,node);

//  	printf("%s",lst->content);
// 	lst = lst->next;
// 	printf("%s",lst->content);

// }
