#include <stdio.h>
#include "fdf.h"

float	rad(int deg)
{
	float	radians;

	radians = deg * PI / 180.0;
	return (radians);
}

int		exit_x(void *par)
{
	(void)par;
	exit(1);
	return (0);
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
/*	else if (keycode == KP_Add)
		hc += 0.100000;
	else if (keycode == KP_Subtract)
		hc -= 0.100000;
	if (hc == 0)
		hc = 0.100000;*/
	draw(p);
	return (0);
}

t_point	*new_point(int x, int y, int z)
{
	t_point		*p;

	if (!(p = (t_point *)malloc(sizeof(t_point))))
		return (NULL);
	p->x = x;
	p->y = y;
	p->z = z;
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
	fdf->cam->alph = 0;
	fdf->cam->beta = 0;
	fdf->cam->gamm = 0;
	fdf->cam->pos = new_point(0, 0, 0);
	fdf->cam->zoom = 30;
	fdf->cam->xoff = WIDTH /4;
	fdf->cam->yoff = HEIGHT /4;
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
	char		**split;
	t_coords	*coords;
	int			*coord_arr;

	coords = NULL;
	if ((fd = open(map_path, O_RDONLY)) < 0)
		return (-1);
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
	while (coords)
	{
		*(coord_arr) = coords->c;
		coords = coords->next;
		coord_arr++;
	}
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
	mlx_hook(fdf->window, 2, 5, key_handler, fdf);
	mlx_loop(fdf->mlx);
	return (0);
}

void	put_pxl(t_mlx *fdf, t_point *p, unsigned int color)
{
	int		i;

	if (p->x > WIDTH || p->y > HEIGHT || p->x < 0 || p->y < 0)
		return;
	i = (p->x * 4) + (p->y * fdf->s_line);
	fdf->pxl[i] = color;
	fdf->pxl[++i] = color >> 8;
	fdf->pxl[++i] = color >> 16;
}

int		get_color(int c)
{
	int		red;

	red = 0xFF0000;
	red += (red >> 8) * c;
	return (red);
}

void	clear_image(void *img, int bpp)
{
	ft_bzero(img, WIDTH * HEIGHT * bpp);
}

void draw_line(t_mlx *fdf, t_point *p0, t_point *p1) {
    const int deltaX = abs(p1->x - p0->x);
    const int deltaY = abs(p1->y - p0->y);
    const int signX = p0->x < p1->x ? 1 : -1;
    const int signY = p0->y < p1->y ? 1 : -1;
    //
    t_point *p;
    int error = deltaX - deltaY;
    //
    while(p0->x != p1->x || p0->y != p1->y) 
	{
		p = (t_point *)malloc(sizeof(t_point));
		p->x = p0->x;
		p->y = p0->y;
		put_pxl(fdf, p, get_color(p0->z));
		const int error2 = error * 2;
		//
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

t_point	*point(int x, int y, int z, t_cam *cam)
{
	float		a = cam->alph;
	float		b = cam->beta;
	float		g = cam->gamm;
	int			gridw;

	t_point		*p;
	gridw = abs(cam->zoom);
	x *= gridw;
	y *= gridw;
	z *= gridw;
	if (!(p = new_point(0, 0, 0)))
		return (NULL);

	p->x = cos(b)*cos(g)*x + cos(b)*sin(g)*y + sin(b)*z;
	p->y = (-sin(a)*sin(b)*cos(g)-cos(a)*sin(g))*x + (-sin(a)*sin(b)*sin(g)+cos(a)*cos(g))*y +
		sin(a)*cos(b)*z;
	p->z = (-sin(b)*cos(a)*cos(g)+sin(a)*sin(g))*x + (-sin(b)*cos(a)*sin(g)-sin(a)*cos(g))*y +
		cos(a)*cos(b)*z;

	p->x += cam->xoff;
	p->y += cam->yoff;
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
			z = coords[x + y * w];
			if (x != w - 1)
				draw_line(fdf, point(x, y, z/10, fdf->cam), point(x + 1, y, coords[x+1+y*w]/10, fdf->cam));
			if (y != h - 1)
				draw_line(fdf, point(x, y, z/10, fdf->cam), point(x, y + 1, coords[x+(y+1)*w]/10, fdf->cam));
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(fdf->mlx, fdf->window, fdf->img, 0, 0);

	return (NULL);
}