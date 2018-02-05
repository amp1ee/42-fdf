#ifndef UTIL_H
# define UTIL_H

typedef struct			s_eulers
{
	float				sin;
	float				cos;
}						t_eulers;

t_eulers				get_eulers(float angle);

#endif