#include "fdf.h"
#include <math.h>

float		rad(float deg)
{
	float	radians;

	radians = deg * PI / 180.0;
	return (radians);
}

t_eulers	get_eulers(float angle)
{
	t_eulers	eul;

	eul.sin = sin(angle);
	eul.cos = cos(angle);
	return (eul);
}

t_point		assign_point(int x, int y)
{
	t_point		p;

	p.x = x;
	p.y = y;
	return (p);
}

int			ind(int x, int y, int w, int h)
{
	return (w * h - (x + y * w) - 1);
}

int			terminate(char *err)
{
	ft_putendl(err);
	exit(0);
	return (0);
}
