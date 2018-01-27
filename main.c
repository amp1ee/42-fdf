#include "fdf.h"

float	rad(float deg)
{
	float	radians;

	radians = deg * PI / 180.0;
	return (radians);
}

t_mlx	*init_fdf(t_map *map)
{
	t_mlx		*fdf;

	if (!(fdf = (t_mlx *)malloc(sizeof(t_mlx))))
		return (NULL);
	if (!(fdf->mlx = mlx_init()) ||
		!(fdf->window = mlx_new_window(fdf->mlx, WIDTH, HEIGHT, "FdF")) ||
		!(fdf->cam = malloc(sizeof(t_cam))) ||
		!(fdf->img = mlx_new_image(fdf->mlx, WIDTH, HEIGHT)) ||
		!(fdf->pxl = mlx_get_data_addr(fdf->img, &(fdf->bpp),
		&(fdf->s_line), &(fdf->endian))))
		return (NULL);
	fdf->map = map;
	fdf->cam->alph = rad(-45);
	fdf->cam->beta = 0;
	fdf->cam->gamm = rad(35.6);
	fdf->cam->zoom = MIN(HEIGHT / (map->max_depth - map->min_depth) + 1,
		((HEIGHT / map->h + WIDTH / map->w) / 4) + 1);
	fdf->cam->xoff = -(map->w * fdf->cam->zoom / 4);
	fdf->cam->yoff = (map->h * fdf->cam->zoom) / 3;
	fdf->cam->isom = 1;
	fdf->cam->zdiv = 1;
	return (fdf);
}

int		print_err(char *err)
{
	ft_putendl(err);
	return (1);
}

int		main(int ac, char **av)
{
	t_mlx		*fdf;
	t_map		*map;
	char		*line;
	t_coords	*c;
	int			fd;

	if (ac == 2)
	{
		if ((fd = open(av[1], O_RDONLY)) < 0 ||
			!(map = read_map(fd, &line, &c)))
			return (print_err(READ_ERR_MSG));
		else if (!(fdf = init_fdf(map)))
			return (print_err(INIT_ERR_MSG));
	}
	else
		return (print_err(USG_ERR_MSG));
	draw(fdf);
	mlx_hook(fdf->window, 4, (1L << 2), mouse_pressed, fdf);
	mlx_hook(fdf->window, 2, 5, key_handler, fdf);
	mlx_loop(fdf->mlx);
	return (0);
}

double	find_perc(double start, double end, double cur)
{
	if (cur == start)
		return (0.0);
	if (cur == end)
		return (1.0);
	return ((cur - start) / (end - start));
}

int			interp_i(int first, int second, double p)
{
	if (first == second)
		return (first);
	return ((int)((1 - p) * first + p * second));
}

int			interp_color(int c1, int c2, double p)
{
	int r;
	int g;
	int b;

	if (c1 == c2)
		return (c1);
	r = interp_i((c1 >> 16) & 0xFF, (c2 >> 16) & 0xFF, p);
	g = interp_i((c1 >> 8) & 0xFF, (c2 >> 8) & 0xFF, p);
	b = interp_i(c1 & 0xFF, c2 & 0xFF, p);
	return (r << 16 | g << 8 | b);
}

t_point		assign_point(int x, int y)
{
	t_point		p;

	p.x = x;
	p.y = y;
	return (p);
}

int		ind(int x, int y, int w, int h)
{
	return (w * h - (x + y * w) - 1);
}

int			get_color(int z, t_mlx m)
{
	int			rgb;
	t_point		color[5];
	int			i;

	color[0] = assign_point(0xdce35b, 0x45b649);
	color[1] = assign_point(0xee0979, 0xff6a00);
	color[2] = assign_point(0xff00cc, 0xff6a00);
	color[3] = assign_point(0xffd89b, 0x19547b);
	color[4] = assign_point(0x22c1c3, 0xfdbb2d);
	i = round(find_perc(m.map->min_depth, m.map->max_depth, z) * 4);
	rgb = interp_color(color[i].x, color[i].y, find_perc(m.map->min_depth,
		(m.map->max_depth / m.cam->zdiv), z));
	return (rgb);
}

t_eulers	get_eulers(float angle)
{
	t_eulers	eul;

	eul.sin = sin(angle);
	eul.cos = cos(angle);
	return (eul);
}
