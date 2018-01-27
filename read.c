#include "fdf.h"
#include <fcntl.h>
#include <stdlib.h>

static inline t_coords	*new_coord(char *num)
{
	t_coords	*new;

	if (!(new = (t_coords *)malloc(sizeof(t_coords))))
	{
		terminate(MALLOC_ERR);
		return (NULL);
	}
	new->c = ft_atoi(num);
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

static t_map			*conv_to_arr(t_map *map, t_coords *coords)
{
	int			*coord_arr;

	coord_arr = map->coord_arr;
	map->w = map->w / map->h;
	while (coords)
	{
		*(coord_arr) = coords->c;
		(coords) = coords->next;
		coord_arr++;
	}
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
			split++;
		}
		map->h += 1;
	}
	close(fd);
	if (rv == -1 || !(map->coord_arr = (int *)malloc(sizeof(int) * map->w)))
		return (NULL);
	return (conv_to_arr(map, *coords));
}
