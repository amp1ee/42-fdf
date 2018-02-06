/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/04 16:40:33 by oahieiev          #+#    #+#             */
/*   Updated: 2018/02/04 16:41:05 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <math.h>

double				find_perc(double start, double end, double cur)
{
	if (start == end)
		return (1.0);
	if (cur == start)
		return (0.0);
	if (cur == end)
		return (1.0);
	return ((cur - start) / (end - start));
}

static int			interp_i(int first, int second, double p)
{
	if (first == second)
		return (first);
	return ((int)((1 - p) * first + p * second));
}

int					interp_color(int c1, int c2, double p)
{
	int r;
	int g;
	int b;

	if (c1 == c2)
		return (c1);
	r = interp_i(c1 >> 16, c2 >> 16, p);
	g = interp_i((c1 >> 8) & 0xFF, (c2 >> 8) & 0xFF, p);
	b = interp_i(c1 & 0xFF, c2 & 0xFF, p);
	return (r << 16 | g << 8 | b);
}

int					get_color(int z, t_map map)
{
	int			rgb;
	t_point		color[10];
	int			i;
	float		lmin;
	float		lmax;

	color[0] = assign_point(0xC02425, 0xCA4729);
	color[1] = assign_point(0xCA4729, 0xD1612A);
	color[2] = assign_point(0xD1612A, 0xDB832E);
	color[3] = assign_point(0xDB832E, 0xE7AB32);
	color[4] = assign_point(0xE7AB32, 0xF0CB35);
	color[5] = assign_point(0x00001D, 0x3355C3);
	color[6] = assign_point(0x3355C3, 0xAA8900);
	color[7] = assign_point(0xAA8900, 0xAA3500);
	color[8] = assign_point(0xAA3500, 0xAA3E0B);
	color[9] = assign_point(0xAA3E0B, 0x6CB5D8);
	i = floor(find_perc(map.min_z, map.max_z, z) * 5);
	lmin = map.min_z + (i * 0.200) * (map.rng);
	lmax = (i == 5) ? map.max_z : map.min_z + (0.2 + i * 0.200) * (map.rng);
	(i == 5) ? i = 4 : 0;
	rgb = interp_color(color[i + map.cm % 10].x, color[i + map.cm % 10].y,
		find_perc(lmin, lmax, z));
	return (rgb);
}
