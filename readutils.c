/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readutils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/04 16:42:33 by oahieiev          #+#    #+#             */
/*   Updated: 2018/02/04 16:42:34 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_coords		*new_coord(char *str)
{
	t_coords	*new;

	if (!(new = (t_coords *)malloc(sizeof(t_coords))))
		return (NULL);
	new->c = ft_atoi(str);
	while (*str && *str != 'x')
		str++;
	new->rgb = (*str) ? ft_atoi_base(str++, 16) : -1;
	new->next = NULL;
	return (new);
}

void			del_list(t_coords **list)
{
	t_coords	*tmp;

	if (!list)
		return ;
	while (*list)
	{
		tmp = *list;
		*list = (*list)->next;
		free(tmp);
		tmp = NULL;
	}
}

int				push_coord(t_coords **head, t_coords *new)
{
	if (!new)
	{
		del_list(head);
		return (0);
	}
	new->next = *head;
	*head = new;
	return (1);
}

int				countwords(char *line, char delim)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (line[i])
	{
		while (line[i] && line[i] == delim)
			i++;
		if (line[i] && line[i] != delim)
		{
			while (line[i] && line[i] != delim)
				i++;
			count++;
		}
	}
	return (count);
}
