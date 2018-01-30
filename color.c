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
	r = interp_i(c1 >> 16, c2 >> 16, p);
	g = interp_i((c1 >> 8) & 0xFF, (c2 >> 8) & 0xFF, p);
	b = interp_i(c1 & 0xFF, c2 & 0xFF, p);
	return (r << 16 | g << 8 | b);
}

int			get_color(int z, t_mlx m)
{
	int			rgb;
	t_point		color[5];
	int			i;

	color[0] = assign_point(0x3d3d45, 0x1f3023);
	color[1] = assign_point(0x1f3023, 0x007565);
	color[2] = assign_point(0x007565, 0x99c6c4);
	color[3] = assign_point(0x99c6c4, 0xefe8cf);
	color[4] = assign_point(0xefe8cf, 0xffffff);
	i = round(find_perc(m.map->min_depth, m.map->max_depth, z) * 4);
	rgb = interp_color(color[i].x, color[i].y, find_perc(m.map->min_depth,
		m.map->max_depth, z));
	return (rgb);
}
