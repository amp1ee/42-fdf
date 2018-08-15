/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/04 16:41:57 by oahieiev          #+#    #+#             */
/*   Updated: 2018/02/04 16:41:59 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <fcntl.h>
#include "mlx.h"
#include <stdlib.h>

void			*cleanall(t_fdf *fdf)
{
	ft_memdel((void **)&(fdf->cam));
	if (fdf->map)
	{
		ft_memdel((void **)&(fdf->map->coord_arr));
		ft_memdel((void **)&(fdf->map->color_arr));
		ft_memdel((void **)&(fdf->map));
	}
	mlx_destroy_window(fdf->mlx, fdf->window);
	// mlx_destroy_image(fdf->mlx, fdf->img);
	ft_bzero(fdf->pxl, WIDTH * HEIGHT * (fdf->bpp / 8));
	ft_memdel((void **)&fdf);
	return (NULL);
}

static t_fdf	*init_fdf(t_map *map)
{
	t_fdf		*fdf;

	if (!(fdf = (t_fdf *)malloc(sizeof(t_fdf))))
		return (NULL);
	if (!(fdf->mlx = mlx_init()) ||
		!(fdf->window = mlx_new_window(fdf->mlx, WIDTH, HEIGHT, "FdF")) ||
		!(fdf->cam = malloc(sizeof(t_cam))) ||
		!(fdf->img = mlx_new_image(fdf->mlx, WIDTH, HEIGHT)) ||
		!(fdf->pxl = mlx_get_data_addr(fdf->img, &(fdf->bpp),
		&(fdf->s_line), &(fdf->endian))))
		return (cleanall(fdf));
	fdf->map = map;
	fdf->cam->alph = rad(ISOMETRIC_ANGLE_A);
	fdf->cam->beta = rad(ISOMETRIC_ANGLE_B);
	fdf->cam->gamm = rad(ISOMETRIC_ANGLE_G);
	fdf->cam->zoom = MIN(HEIGHT / abs(map->rng + 1),
		((HEIGHT / map->h + WIDTH / map->w) / 4) + 1);
	fdf->cam->zoom <= 0 ? fdf->cam->zoom = 1 : 0;
	fdf->cam->xoff = -(map->w * fdf->cam->zoom / 4);
	fdf->cam->yoff = (map->h * fdf->cam->zoom) / 3;
	fdf->cam->isom = 1;
	fdf->cam->zdiv = 1.0;
	fdf->showhelp = 0;
	return (fdf);
}

int				main(int ac, char **av)
{
	t_fdf		*fdf;
	t_map		*map;
	int			fd;

	if (ac == 2)
	{
		if ((fd = open(av[1], O_RDONLY)) < 0 ||
			!(map = read_map(fd)))
			return (terminate(READ_ERR_MSG));
		else
			close(fd);
		if (!(fdf = init_fdf(map)))
			return (terminate(INIT_ERR_MSG));
	}
	else
		return (terminate(USG_ERR_MSG));
	draw(fdf, (*(*fdf).map).w, (*(*fdf).map).h);
	mlx_hook(fdf->window, 4, (1L << 2), mouse_pressed, fdf);
	mlx_hook(fdf->window, 2, 5, key_pressed, fdf);
	mlx_loop(fdf->mlx);
	return (0);
}
