#include <stdio.h>
#include "fdf.h"

float	rad(float deg)
{
	float	radians;

	radians = deg * PI / 180.0;
	return (radians);
}

int		key_handler(int keycode, void *param)
{
	t_mlx	*p;

	p = (t_mlx *)param;
	//printf("key = %d\n", keycode);
	if (keycode == KB_Esc)
	{
		mlx_destroy_window(p->mlx, p->window);
		ft_memdel((void **)&p);
		p = NULL;
		exit(0);
		return (0);
	}
	else if (keycode == KB_Up)
		p->cam->alph -= rad(2);
	else if (keycode == KB_Down)
		p->cam->alph += rad(2);
	else if (keycode == KB_Left)
		p->cam->beta -= rad(2);
	else if (keycode == KB_Right)
		p->cam->beta += rad(2);
	else if (keycode == KB_W)
		p->cam->gamm -= rad(2);
	else if (keycode == KB_S)
		p->cam->gamm += rad(2);
	else if (keycode == KB_X)
		p->cam->zoom++;
	else if (keycode == KB_Z)
		p->cam->zoom--;
	else if (keycode == KP_4)
		p->cam->xoff -= 10;
	else if (keycode == KP_6)
		p->cam->xoff += 10;
	else if (keycode == KP_8)
		p->cam->yoff -= 10;
	else if (keycode == KP_2)
		p->cam->yoff += 10;
	else if (keycode == KB_i)
	{
		p->cam->alph = 0;
		p->cam->beta = 0;
		p->cam->gamm = 0;
	}	
/*	else if (keycode == KP_Add)
		hc += 0.100000;
	else if (keycode == KP_Subtract)
		hc -= 0.100000;
	if (hc == 0)
		hc = 0.100000;*/
	if (p->cam->zoom == 0)
		p->cam->zoom = 1;
	draw(p);
	return (0);
}

t_point	*new_point(int x, int y, int z, int rgb)
{
	t_point		*p;

	if (!(p = (t_point *)malloc(sizeof(t_point))))
		return (NULL);
	p->x = x;
	p->y = y;
	p->z = z;
	p->rgb = rgb;
	return (p);
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
	if (!(fdf->cam = (t_cam *)malloc(sizeof(t_cam))))
		return (NULL);
	fdf->cam->alph = rad(-35.264);
	fdf->cam->beta = 0;
	fdf->cam->gamm = rad(45);
	fdf->cam->pos = new_point(0, 0, 0, 0);
	fdf->cam->zoom = 30;
	fdf->cam->xoff = WIDTH /4;
	fdf->cam->yoff = HEIGHT /4;
	return (fdf);
}

void		find_range(t_mlx *p)
{
	int			min;
	int			max;
	t_point		v;
	int			z;

	min = 2147483647;
	max = -2147483648;
	v.y = 0;
	while (v.y < p->h)
	{
		v.x = 0;
		while (v.x < p->w)
		{
			z = p->coord_arr[(int)v.y * p->w + (int)v.x];
			if (z < min)
				min = z;
			if (z > max)
				max = z;
			v.x++;
		}
		v.y++;
	}
	p->min_depth = min;
	p->max_depth = max;
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
	if (!new || !head)
		return;
	new->next = *head;
	*head = new;
}

int		read_map(char *map_path, t_mlx *fdf)
{
	int			fd;
	char		*line;
	char		**split;
	t_coords	*coords;
	t_coords	**head;
	int			*coord_arr;

	coords = NULL;
	head = &coords;
	if ((fd = open(map_path, O_RDONLY)) < 0)
		return (-1);
	printf("reading the map\n");
	while (get_next_line(fd, &line) > 0)
	{
		split = ft_strsplit(line, ' ');
		ft_strdel(&line);
		while (*split != NULL)
		{
			push_coord(&coords, new_coord(*split));
			fdf->w += 1;
			split++;
		}
		fdf->h += 1;
	}
	fdf->coord_arr = (int *)malloc(sizeof(int) * fdf->w);
	coord_arr = fdf->coord_arr;
	fdf->w = fdf->w / fdf->h;
	printf("w = %d, h = %d\n", fdf->w, fdf->h);
	printf("converting list to arr\n");
	while (*head)
	{
		*(coord_arr) = (*head)->c;
		(*head) = (*head)->next;
		coord_arr++;
	}
	printf("converted\n");
	close(fd);
	find_range(fdf);
	return (0);
}

int		main(int ac, char **av)
{
	t_mlx		*fdf;

	fdf = init_fdf();
	if (ac == 2)
		read_map(av[1], fdf);

	draw(fdf);
	mlx_hook(fdf->window, 2, 5, key_handler, fdf);
	mlx_loop(fdf->mlx);
	return (0);
}

void	put_pxl(t_mlx *fdf, t_point *p, unsigned int color)
{
	int		i;

	if (p->x >= WIDTH || p->y >= HEIGHT || p->x < 0 || p->y < 0)
		return;
	i = (p->x * 4) + (p->y * fdf->s_line);
	fdf->pxl[i] = color;
	fdf->pxl[++i] = color >> 8;
	fdf->pxl[++i] = color >> 16;
}

void	clear_image(void *img, int bpp)
{
	ft_bzero(img, WIDTH * HEIGHT * bpp);
}


double	find_perc(double cur, double start, double end)
{
	if (cur == start)
		return (0.0);
	if (cur == end)
		return (1.0);
	return ((cur - start) / (end - start));
}
int		add_perc(int first, int second, double p)
{
	if (first == second)
		return (first);
	return ((int)((double)first + (second - first) * p));
}
int		get_color(int c1, int c2, double p)
{
	int r;
	int g;
	int b;

	if (c1 == c2)
		return (c1);
	r = add_perc((c1 >> 16) & 0xFF, (c2 >> 16) & 0xFF, p);
	g = add_perc((c1 >> 8) & 0xFF, (c2 >> 8) & 0xFF, p);
	b = add_perc(c1 & 0xFF, c2 & 0xFF, p);
	return (r << 16 | g << 8 | b);
}

int		get_grad(int z, t_mlx m)
{
	int			rgb;

	rgb = get_color(0x009BDF, 0xFF0000, find_perc(z,
				m.min_depth, m.max_depth));
	return (rgb);
}

void	draw_line(t_mlx *fdf, t_point *p0, t_point *p1) {
    const int 	deltaX = abs(p1->x - p0->x);
    const int 	deltaY = abs(p1->y - p0->y);
    const int 	signX = p0->x < p1->x ? 1 : -1;
    const int 	signY = p0->y < p1->y ? 1 : -1;
    int 		error = deltaX - deltaY;
    double	perc;
   	t_point		p = *p0;

    while(p0->x != p1->x || p0->y != p1->y) 
	{
		perc = (deltaX > deltaY ?
			find_perc((int)p0->x, (int)p.x, (int)p1->x)
			: find_perc((int)p0->y, (int)p.y, (int)p1->y));
		put_pxl(fdf, p0, get_color(p0->rgb,
				p1->rgb, perc));
		const int error2 = error * 2;
		if (error2 > -deltaY) 
        {
            error -= deltaY;
            p0->x += signX;
        }
        if (error2 < deltaX)
        {
            error += deltaX;
            p0->y += signY;
        }
    }
}

t_eulers	get_eulers(float angle)
{
	t_eulers	eul;

	eul.sin = sin(angle);
	eul.cos = cos(angle);
	return (eul);
}

t_point		*project(int x, int y, int z, t_mlx *mlx)
{
	t_eulers	a = get_eulers(mlx->cam->alph);
	t_eulers	b = get_eulers(mlx->cam->beta);
	t_eulers	g = get_eulers(mlx->cam->gamm);
	int			gridw;
	t_point		*p;

	gridw = abs(mlx->cam->zoom);
	x *= gridw;
	y *= gridw;
	z *= gridw;
	if (!(p = new_point(0, 0, 0, 0)))
		return (NULL);
	p->rgb = get_grad(z / gridw, *mlx);
	p->x = b.cos * g.cos * x + b.cos * g.sin * y + b.sin * z;
	p->y = (-a.sin * b.sin * g.cos - a.cos * g.sin) * x +
		(a.cos * g.cos - a.sin * b.sin * g.sin) * y + a.sin * b.cos * z;
	p->z = (-b.sin * a.cos * g.cos + a.sin * g.sin) * x + (-b.sin *
		a.cos * g.sin - a.sin * g.cos) * y + a.cos * b.cos * z;
	p->x += mlx->cam->xoff;
	p->y += mlx->cam->yoff;
	return (p);
}

void	*draw(t_mlx *fdf)
{
	int			x, y;
	int			w, h;
	int			*coords;
	int			z;

	clear_image(fdf->pxl, fdf->bpp /8);
	coords = fdf->coord_arr;
	w = fdf->w;
	h = fdf->h;
	y = 0;
	while (y < h)
	{
		x = 0;
		while (x < w)
		{
			z = coords[w * h - (x + y * w)];
			if (x != w - 1)
				draw_line(fdf, project(x, y, z, fdf), project(x + 1, y, coords[w*h-(x+1+y*w)], fdf));
			if (y != h - 1)
				draw_line(fdf, project(x, y, z, fdf), project(x, y + 1, coords[w*h-(x+(y+1)*w)], fdf));
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(fdf->mlx, fdf->window, fdf->img, 0, 0);
	return (NULL);
}