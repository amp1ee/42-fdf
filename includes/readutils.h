/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readutils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/04 16:52:38 by oahieiev          #+#    #+#             */
/*   Updated: 2018/02/04 16:52:39 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READUTILS_H
# define READUTILS_H

typedef	struct			s_coords
{
	int					c;
	int					rgb;
	struct s_coords		*next;
}						t_coords;

t_coords		*new_coord(char *str);
void			del_list(t_coords **list);
int				push_coord(t_coords **head, t_coords *new);
int				countwords(char *line, char delim);

#endif
