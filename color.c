#include "fdf.h"
#include <math.h>

double		find_perc(double start, double end, double cur)
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
