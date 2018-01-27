#include "fdf.h"
#include <stdlib.h>

static inline t_coords	*new_coord(char *str)
{
	t_coords	*new;

	if (!(new = (t_coords *)malloc(sizeof(t_coords))))
	{
		terminate(MALLOC_ERR);
		return (NULL);
	}
	new->c = ft_atoi(str);
	new->next = NULL;
	return (new);
}

static inline void		push_coord(t_coords **head, t_coords *new)
{
	if (!new || !head)
		return ;
	new->next = *head;
	*head = new;
}

static void				find_range(t_map *map)
{
	int			min;
	int			max;
	t_point		p;
	int			z;

	min = 2147483647;
	max = -2147483648;
	p.y = 0;
	while (p.y < map->h)
	{
		p.x = 0;
		while (p.x < map->w)
		{
			z = map->coord_arr[(int)p.y * map->w + (int)p.x];
			if (z < min)
				min = z;
			if (z > max)
				max = z;
			p.x++;
		}
		p.y++;
	}
	map->min_depth = min;
	map->max_depth = max;
}

void					del_list(t_coords **list)
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

static t_map			*conv_to_arr(t_map *map, t_coords *coords)
{
	int			*coord_arr;
	t_coords	*coords_head;
	int			i;

	i = 0;
	coords_head = coords;
	coord_arr = map->coord_arr;
	map->w = map->w / map->h;
	while (coords)
	{
		coord_arr[i++] = coords->c;
		coords = coords->next;
	}
	del_list(&coords_head);
	find_range(map);
	return (map);
}

t_map					*read_map(int fd, char **line, t_coords **coords)
{
	int			rv;
	char		**split;
	t_map		*map;

	if (!(map = malloc(sizeof(t_map))))
		return (NULL);
	*coords = NULL;
	map->w = 0;
	map->h = 0;
	while ((rv = get_next_line(fd, line)) > 0)
	{
		split = ft_strsplit(*line, ' ');
		ft_strdel(line);
		while (*split != NULL)
		{
			push_coord(coords, new_coord(*split));
			map->w += 1;
			ft_strdel(&(*(split++)));
		}
		map->h += 1;
		ft_memdel((void **)(&split - sizeof(split) * (map->w / map->h)));
	}
	if (rv == -1 || !(map->coord_arr = (int *)malloc(sizeof(int) * map->w)))
		return (NULL);
	return (conv_to_arr(map, *coords));
}
