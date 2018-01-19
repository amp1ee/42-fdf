#include <stdio.h>
#include "fdf.h"

int		key_handler(int keycode, void *param)
{
	t_mlx	*p;

	p = (t_mlx *)param;
	if (keycode == KB_Esc)
	{
		printf("Escape\n");
		mlx_destroy_window(p->mlx, p->window);
		p = NULL;
		exit(0);
	}
	draw(p);
	return (0);
}

t_mlx	*init_fdf(void)
{
	void		*mlx;
	void		*window;
	t_mlx		*fdf;

	if (!(fdf = (t_mlx *)malloc(sizeof(t_mlx))))
		return (NULL);
	mlx = mlx_init();
	window = mlx_new_window(mlx, WIDTH, HEIGHT, "Fils de fer");
	fdf->mlx = mlx;
	fdf->window = window;
	fdf->img = mlx_new_image(mlx, WIDTH, HEIGHT);
	fdf->pxl = mlx_get_data_addr(fdf->img, &(fdf->bpp), &(fdf->s_line), &(fdf->ed));
	fdf->w = 0;
	fdf->h = 0;
	fdf->coords = NULL;
	return (fdf);
}

t_coords	*new_coord(char *line)
{
	t_coords	*new;

	new = (t_coords *)malloc(sizeof(t_coords));
	new->c = ft_atoi(line);
	new->next = NULL;
	return (new);
}

void		push_coord(t_coords **head, t_coords *new)
{
	t_coords	*n;

	n = *head;
	if (!(*head))
		*head = new;
	else
	{
		while (n->next)
			n = n->next;
		n->next = new;
	}
}

int		read_map(char *map_path, t_mlx *fdf)
{
	int			fd;
	char		*line;
	char		**coords;
	
	fd = open(map_path, O_RDONLY);
	if (fd < 0)
		return (-1);
	while (get_next_line(fd, &line) > 0)
	{
		coords = ft_strsplit(line, ' ');
		while (*coords != NULL)
		{
			push_coord(&fdf->coords, new_coord(*coords));
			fdf->w += 1;
			coords++;
		}
		fdf->h += 1;
	}
	fdf->w = fdf->w / fdf->h;
/*	printf("w = %d, h = %d\n", fdf->w, fdf->h);
	while (fdf->coords)
	{
		printf("c = %d\n", fdf->coords->c);
		fdf->coords = fdf->coords->next;
	}*/
	close(fd);
	return (0);
}

int		main(int ac, char **av)
{
	t_mlx		*fdf;

	fdf = init_fdf();
	if (ac == 2)
		read_map(av[1], fdf);
	draw(fdf);
	mlx_key_hook(fdf->window, &key_handler, fdf);
	mlx_loop(fdf->mlx);
	return (0);
}

void	put_pxl(t_mlx *e, int x, int y, unsigned int c)
{
	int		i;

	if (x > WIDTH)
		x = WIDTH;
	if (y > HEIGHT)
		y = HEIGHT;
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	i = (x * 4) + (y * e->s_line);
	e->pxl[i] = c;
	e->pxl[++i] = c >> 8;
	e->pxl[++i] = c >> 16;
}

void	*draw(t_mlx *fdf)
{
	int		x, y;

	x = 0;
	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
			if (y > 0 && !(x*x % y))
				put_pxl(fdf, x, y, 0xabcdef);
			y++;
		}
		x++;
	}
	mlx_put_image_to_window(fdf->mlx, fdf->window, fdf->img, 0, 0);

	return (NULL);
}