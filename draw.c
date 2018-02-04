/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/04 16:41:46 by oahieiev          #+#    #+#             */
/*   Updated: 2018/02/04 16:41:48 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <mlx.h>

static void		put_pxl(t_mlx *fdf, t_point p, unsigned int color)
{
	int		i;

	if (p.x >= WIDTH || p.y >= HEIGHT || p.x < 0 || p.y < 0)
		return ;
	i = (p.x * 4) + (p.y * fdf->s_line);
	fdf->pxl[i] = color;
	fdf->pxl[++i] = color >> 8;
	fdf->pxl[++i] = color >> 16;
}

static void		draw_ln(t_mlx *fdf, t_point p0, t_point p1)
{
	t_point		delta;
	t_point		sign;
	int			error[2];
	t_point		p;

	delta = assign_point(abs(p1.x - p0.x), abs(p1.y - p0.y));
	sign = assign_point(p0.x < p1.x ? 1 : -1, p0.y < p1.y ? 1 : -1);
	error[0] = delta.x - delta.y;
	p = p0;
	while (p0.x != p1.x || p0.y != p1.y)
	{
		put_pxl(fdf, p0, interp_color(p0.rgb, p1.rgb, (delta.x > delta.y ?
			find_perc(p.x, p1.x, p0.x) : find_perc(p.y, p1.y, p0.y))));
		error[1] = error[0] * 2;
		if (error[1] > -delta.y)
		{
			error[0] -= delta.y;
			p0.x += sign.x;
		}
		if (error[1] < delta.x)
		{
			error[0] += delta.x;
			p0.y += sign.y;
		}
	}
}

static t_point	project(int x, int y, int z, t_mlx *mlx)
{
	t_eulers	a;
	t_eulers	b;
	t_eulers	g;
	t_point		p;

	p.rgb = mlx->map->color_arr[ind(x, y, mlx->map->w, mlx->map->h)];
	(p.rgb == -1) ? p.rgb = get_color(z, *(mlx->map)) : 0;
	a = get_eulers(mlx->cam->alph);
	b = get_eulers(mlx->cam->beta);
	g = get_eulers(mlx->cam->gamm);
	x *= mlx->cam->zoom;
	y *= mlx->cam->zoom;
	z *= (mlx->cam->zoom / mlx->cam->zdiv);
	p.x = b.cos * g.cos * x + b.cos * g.sin * y + b.sin * z;
	p.y = (-a.sin * b.sin * g.cos - a.cos * g.sin) * x +
		(a.cos * g.cos - a.sin * b.sin * g.sin) * y + a.sin * b.cos * z;
	p.z = (-b.sin * a.cos * g.cos + a.sin * g.sin) * x + (-b.sin *
		a.cos * g.sin - a.sin * g.cos) * y + a.cos * b.cos * z;
	p.x += (WIDTH - mlx->cam->zoom * mlx->map->w) / 2 + mlx->cam->xoff;
	p.y += HEIGHT / 2 + mlx->cam->yoff;
	return (p);
}

static void		print_controls(t_mlx *fdf)
{
	int		y;

	y = 15;
	mlx_string_put(fdf->mlx, fdf->window, 15, y += 15, TXT_COLOR, "CONTROLS:");
	mlx_string_put(fdf->mlx, fdf->window, 15, y += 15, TXT_COLOR,
		"Mouse wheel Up/Down - Zoom In/Out");
	mlx_string_put(fdf->mlx, fdf->window, 15, y += 15, TXT_COLOR,
		"Left mouse button - Place at cursor position");
	mlx_string_put(fdf->mlx, fdf->window, 15, y += 15, TXT_COLOR,
		"Up/Down, Left/Right, W/S - Rotate");
	mlx_string_put(fdf->mlx, fdf->window, 15, y += 15, TXT_COLOR,
		"Numpad -/+ - Flatten");
	mlx_string_put(fdf->mlx, fdf->window, 15, y += 15, TXT_COLOR,
		"Numpad 2/4/6/8 - Move");
	mlx_string_put(fdf->mlx, fdf->window, 15, y += 15, TXT_COLOR,
		"I - Switch Top/Isometric View");
}

void			draw(t_mlx *fdf, int w, int h)
{
	t_point		p;
	int			*coords;

	ft_bzero(fdf->pxl, WIDTH * HEIGHT * (fdf->bpp / 8));
	coords = fdf->map->coord_arr;
	p.y = 0;
	while (p.y < h)
	{
		p.x = 0;
		while (p.x < w)
		{
			p.z = coords[ind(p.x, p.y, w, h)];
			if (p.x != w - 1)
				draw_ln(fdf, project(p.x, p.y, p.z, fdf), project(p.x + 1, p.y,
				coords[ind(p.x + 1, p.y, w, h)], fdf));
			if (p.y != h - 1)
				draw_ln(fdf, project(p.x, p.y, p.z, fdf), project(p.x, p.y + 1,
				coords[ind(p.x, p.y + 1, w, h)], fdf));
			p.x++;
		}
		p.y++;
	}
	mlx_put_image_to_window(fdf->mlx, fdf->window, fdf->img, 0, 0);
	print_controls(fdf);
}
