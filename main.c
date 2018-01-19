#include <stdio.h>
#include "fdf.h"

float	rad(int deg)
{
	float	radians;

	radians = deg * PI / 180.0;
	return (radians);
}

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
		return (0);
	}
	else if (keycode == KB_Up)
	{
		mlx_clear_window(p->mlx, p->window);
		p->alph += rad(2);
	}
	else if (keycode == KB_Down)
		p->alph -= rad(2);
	else if (keycode == KB_Left)
		p->beta += rad(2);
	else if (keycode == KB_Right)
		p->beta -= rad(2);

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
	printf("w = %d, h = %d\n", fdf->w, fdf->h); 
/*	while (fdf->coords)
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

int		get_color(int c)
{
	int		red;

	red = 0xFF0000;
	red += (red >> 8) * c;
	return (red);
}

void	*draw(t_mlx *fdf)
{
	int			x, y;
	int			w, h;
	int			xw, yh;
	t_coords	*coords;
	float		alph;
	float		beta;

	coords = fdf->coords;
	w = fdf->w;
	h = fdf->h;
	xw = 30;
	yh = 30;
	int xoff = 120;
	int yoff = 60;
	alph = fdf->alph;
	beta = fdf->beta;
	y = 0;
	while (y < h)
	{
		x = 0;
		while (x < w && coords)
		{
			if (coords->c > 0)
				put_pxl(fdf, x*xw*cos(alph) + y*yh*sin(alph) + xoff, (y - coords->c/3)*yh*cos(beta) + x*xw*sin(beta) + yoff, get_color(coords->c));
			else
				put_pxl(fdf,x*xw*cos(alph) + y*yh*sin(alph) + xoff, y*yh*cos(beta) + x*xw*sin(beta) + yoff, 0xFFFFFF);
			coords = coords->next;
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(fdf->mlx, fdf->window, fdf->img, 0, 0);

	return (NULL);
}