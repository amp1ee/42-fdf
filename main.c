#include <stdio.h>
#include "fdf.h"

int hc = 3;

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
	if (keycode == KB_Esc)
	{
		printf("Escape\n");
		mlx_destroy_window(p->mlx, p->window);
		ft_memdel((void **)&p);
		p = NULL;
		exit(0);
		return (0);
	}
	else if (keycode == KB_Left)
		p->alph -= rad(2);
	else if (keycode == KB_Right)
		p->alph += rad(2);
	else if (keycode == KB_Up)
		p->beta -= rad(2);
	else if (keycode == KB_Down)
		p->beta += rad(2);
	else if (keycode == KP_Add)
		hc++;
	else if (keycode == KP_Subtract)
		hc--;
	if (hc == 0)
		hc = 1;
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
	fdf->alph = 0;
	fdf->beta = 0;
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
	fd = open(map_path, O_RDONLY);
	if (fd < 0)
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
	//draw(fdf);
	mlx_key_hook(fdf->window, key_handler, fdf);
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

void draw_line(int z, t_mlx *fdf, t_point *p0, t_point *p1) {
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
		put_pxl(fdf, p, get_color(z * 3));
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

t_point	*point(int x, int y, int z, t_mlx *fdf)
{
	float		alph;
	float		beta;
	int			xw, yh;

	t_point		*p;
	alph = fdf->alph;
	beta = fdf->beta;
	xw = 30;
	yh = 30;
	int 	xoff = 120;
	int 	yoff = 60;
	if (!(p = (t_point *)malloc(sizeof(t_point))))
		return (NULL);
	p->x = x * xw * cos(alph) + y * yh * sin(alph) + xoff;
	p->y = (y - z) * yh * cos(beta) + x * xw * sin(beta) + yoff;
	return (p);
}

void	*draw(t_mlx *fdf)
{
	int			x, y;
	int			w, h;
	int			*coords;

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
			if (coords[x + y * w] > 0)
				put_pxl(fdf, point(x, y, coords[x + y * w]/hc, fdf), get_color(coords[x + y * w]));
			else
				put_pxl(fdf, point(x, y, 0, fdf), 0xFFFFFF);
			if (x != w - 1)
				draw_line(coords[x + y * w], fdf, point(x, y, coords[x + y * w]/hc, fdf), point(x + 1, y, coords[x+1+y*w]/hc, fdf));
			if (y != h - 1)
				draw_line(coords[x + y * w], fdf, point(x, y, coords[x + y * w]/hc, fdf), point(x, y + 1, coords[x+(y+1)*w]/hc, fdf));
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(fdf->mlx, fdf->window, fdf->img, 0, 0);

	return (NULL);
}