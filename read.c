#include "fdf.h"
#include <stdlib.h>

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

static inline t_coords	*new_coord(char *str)
{
	t_coords	*new;

	if (!(new = (t_coords *)malloc(sizeof(t_coords))))
		return (NULL);
	new->c = ft_atoi(str);
	new->next = NULL;
	return (new);
}

static inline int		push_coord(t_coords **head, t_coords *new)
{
	if (!new)
	{
		del_list(head);
		return (terminate(MALLOC_ERR));
	}
	new->next = *head;
	*head = new;
	return (1);
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
			z = map->coord_arr[p.y * map->w + p.x];
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

int						conv_to_arr(t_map *map, t_coords *coords)
{
	int			*coord_arr;
	t_coords	**coords_head;
	int			i;

	if (!(map->coord_arr = (int *)malloc(sizeof(int) * map->w * map->h)))
		return (terminate(MALLOC_ERR));
	i = 0;
	coords_head = &coords;
	coord_arr = map->coord_arr;
	while (coords)
	{
		coord_arr[i++] = coords->c;
		coords = coords->next;
	}
	del_list(coords_head);
	return (1);
}

t_map					*init_map(void)
{
	t_map		*map;

	if (!(map = malloc(sizeof(t_map))))
		return (NULL);
	map->w = 0;
	map->h = 0;
	return (map);
}

int						read_split(char **split, t_map *map, t_coords **coords)
{
	char	**split_h;	

	split_h = split;
	while (*split != NULL)
	{
		if (!(push_coord(coords, new_coord(*split))))
			return (0);
		map->w += 1;
		ft_strdel(&(*(split++)));
	}
	map->h += 1;
	ft_memdel((void **)&split_h);
	return (1);
}

t_map					*read_map(int fd)
{
	int			rv;
	char		**split;
	t_map		*map;
	char		*line;
	t_coords	*coords;

	coords = NULL;
	if (!(map = init_map()))
	{
		terminate(MALLOC_ERR);
		return (NULL);
	}
	while ((rv = get_next_line(fd, &line)) > 0)
	{
		split = ft_strsplit(line, ' ');
		ft_strdel(&line);
		if (!(read_split(split, map, &coords)))
			return (NULL);
	}
	if (rv == -1)
		return (NULL);
	map->w = map->w / map->h;
	conv_to_arr(map, coords);
	find_range(map);
	return (map);
}
