#include "fdf.h"
#include <fcntl.h>
#include <mlx.h>
#include <stdlib.h>

void	*cleanall(t_mlx *fdf)
{
	ft_memdel((void **)&(fdf->cam));
	if (fdf->map)
	{
		ft_memdel((void **)&(fdf->map->coord_arr));
		ft_memdel((void **)&(fdf->map));
	}
	mlx_destroy_window(fdf->mlx, fdf->window);
//	mlx_destroy_image(fdf->mlx, fdf->img);
	ft_bzero(fdf->pxl, WIDTH * HEIGHT * (fdf->bpp / 8));
	free(fdf);
	fdf = NULL;
	return (NULL);
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
		return (cleanall(fdf));
	fdf->map = map;
	fdf->cam->alph = rad(-45);
	fdf->cam->beta = 0;
	fdf->cam->gamm = rad(35.264);
	fdf->cam->zoom = MIN(HEIGHT / (map->max_depth - map->min_depth) + 1,
		((HEIGHT / map->h + WIDTH / map->w) / 4) + 1);
	fdf->cam->xoff = -(map->w * fdf->cam->zoom / 4);
	fdf->cam->yoff = (map->h * fdf->cam->zoom) / 3;
	fdf->cam->isom = 1;
	fdf->cam->zdiv = 1;
	return (fdf);
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
			return (terminate(READ_ERR_MSG));
		else
			close(fd);
		if (!(fdf = init_fdf(map)))
			return (terminate(INIT_ERR_MSG));
	}
	else
		return (terminate(USG_ERR_MSG));
	draw(fdf);
	mlx_hook(fdf->window, 4, (1L << 2), mouse_pressed, fdf);
	mlx_hook(fdf->window, 2, 5, key_pressed, fdf);
	mlx_loop(fdf->mlx);
	return (0);
}
