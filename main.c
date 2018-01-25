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
		p->cam->yoff -= (p->w * p->cam->zoom /2) * ((p->cam->isom) ? 1 : -1);
		p->cam->alph = p->cam->isom ? 0 : rad(-35.264);
		p->cam->beta = 0;
		p->cam->gamm = p->cam->isom ? 0 : rad(45);
		p->cam->isom = !p->cam->isom;
	}	
	else if (keycode == KP_Add)
		p->cam->zdiv++;
	else if (keycode == KP_Subtract)
		p->cam->zdiv--;
	if (p->cam->zdiv == 0)
		p->cam->zdiv = 1;
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
	t_mlx		*fdf;

	if (!(fdf = (t_mlx *)malloc(sizeof(t_mlx))))
		return (NULL);
	if (!(fdf->mlx = mlx_init()) || !(fdf->window = mlx_new_window(fdf->mlx,
		WIDTH, HEIGHT, "FdF")) || !(fdf->cam = malloc(sizeof(t_cam))) ||
	!(fdf->img = mlx_new_image(fdf->mlx, WIDTH, HEIGHT)) || !(fdf->pxl =
		mlx_get_data_addr(fdf->img, &(fdf->bpp), &(fdf->s_line), &(fdf->ed))))
		return (NULL);
	fdf->w = 0;
	fdf->h = 0;
	fdf->cam->alph = 0;
	fdf->cam->beta = 0;
	fdf->cam->gamm = 0;
	fdf->cam->zoom = 30;
	fdf->cam->xoff = 0;
	fdf->cam->yoff = 0;
	fdf->cam->isom = 0;
	fdf->cam->zdiv = 1;
	return (fdf);
}

void		find_range(t_mlx *mlx)
{
	int			min;
	int			max;
	t_point		p;
	int			z;

	min = 2147483647;
	max = -2147483648;
	p.y = 0;
	while (p.y < mlx->h)
	{
		p.x = 0;
		while (p.x < mlx->w)
		{
			z = mlx->coord_arr[(int)p.y * mlx->w + (int)p.x];
			if (z < min)
				min = z;
			if (z > max)
				max = z;
			p.x++;
		}
		p.y++;
	}
	mlx->min_depth = min;
	mlx->max_depth = max;
}

t_coords	*new_coord(char *line)
{
	t_coords	*new;

	if (!(new = (t_coords *)malloc(sizeof(t_coords))))
		return (NULL);
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
	int			rv;
	char		*line;
	char		**split;
	t_coords	*coords;
	t_coords	**head;
	int			*coord_arr;

	coords = NULL;
	head = &coords;
	if ((fd = open(map_path, O_RDONLY)) < 0)
		return (0);
	while ((rv = get_next_line(fd, &line)) > 0)
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
	if (rv == -1 || !(fdf->coord_arr = (int *)malloc(sizeof(int) * fdf->w)))
		return (0);
	coord_arr = fdf->coord_arr;
	printf("w = %d, h = %d\n", fdf->w, fdf->h);
	fdf->w = fdf->w / fdf->h;
	while (*head)
	{
		*(coord_arr) = (*head)->c;
		(*head) = (*head)->next;
		coord_arr++;
	}
	close(fd);
	find_range(fdf);
	printf("min_depth: %d, max_depth: %d\n", fdf->min_depth, fdf->max_depth);
	return (1);
}

int		mouse_pressed(int button, int mouseX, int mouseY, void *param)
{
	t_mlx				*fdf;

	fdf = (t_mlx *)param;
	(void)mouseX;
	if (mouseY < 0)
		return (1);
	else if (button == LMB)
	{
		fdf->cam->xoff = mouseX - WIDTH/2;
		fdf->cam->yoff = mouseY - HEIGHT/2;
	}
	else if (button == MWD)
		fdf->cam->zoom--;
	else if (button == MWU)
	{
		fdf->cam->zoom++;
	}
	if (fdf->cam->zoom == 0)
		fdf->cam->zoom = 1;
	draw(fdf);
	return (0);
}

int		main(int ac, char **av)
{
	t_mlx		*fdf;

	fdf = init_fdf();
	if (fdf != NULL)
	{
		if (!read_map(av[ac - 1], fdf))
		{
			ft_putendl("read err");
			return (0);
		}
	}
	else if (fdf == NULL)
	{
		ft_putendl(INIT_ERR_MSG);
		return (1);
	}
	draw(fdf);
	mlx_hook(fdf->window, 4, (1L<<2), mouse_pressed, fdf);
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

	rgb = get_color(0xFFFFFF, 0xFF0000, find_perc(z,
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

	gridw = mlx->cam->zoom;
	x *= gridw;
	y *= gridw;
	z *= gridw / mlx->cam->zdiv;
	if (!(p = new_point(0, 0, 0, 0)))
		return (NULL);
	p->rgb = get_grad(z / gridw, *mlx);
	p->x = b.cos * g.cos * x + b.cos * g.sin * y + b.sin * z;
	p->y = (-a.sin * b.sin * g.cos - a.cos * g.sin) * x +
		(a.cos * g.cos - a.sin * b.sin * g.sin) * y + a.sin * b.cos * z;
	p->z = (-b.sin * a.cos * g.cos + a.sin * g.sin) * x + (-b.sin *
		a.cos * g.sin - a.sin * g.cos) * y + a.cos * b.cos * z;
	p->x += (WIDTH - mlx->cam->zoom * mlx->w)/2 + mlx->cam->xoff;
	p->y += (HEIGHT - mlx->cam->zoom * mlx->h)/2 + mlx->cam->yoff;
	return (p);
}

int		ind(int x, int y, int w, int h)
{
	return (w * h - (x + y * w) - 1);
}

void	draw(t_mlx *fdf)
{
	t_point		p;
	int			*coords;

	clear_image(fdf->pxl, fdf->bpp /8);
	coords = fdf->coord_arr;
	p.y = 0;
	while (p.y < (*fdf).h)
	{
		p.x = 0;
		while (p.x < (*fdf).w)
		{
			p.z = coords[ind(p.x, p.y, (*fdf).w, (*fdf).h)];
			if (p.x != (*fdf).w - 1)
				draw_line(fdf, project(p.x, p.y, p.z, fdf), project(p.x + 1, p.y,
					coords[ind(p.x + 1, p.y, (*fdf).w, (*fdf).h)], fdf));
			if (p.y != (*fdf).h - 1)
				draw_line(fdf, project(p.x, p.y, p.z, fdf), project(p.x, p.y + 1,
					coords[ind(p.x, p.y + 1, (*fdf).w, (*fdf).h)], fdf));
			p.x++;
		}
		p.y++;
	}
	mlx_put_image_to_window(fdf->mlx, fdf->window, fdf->img, 0, 0);
}



















