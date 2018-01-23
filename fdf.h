#ifndef FDF_H
#define FDF_H

#include <mlx.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include "../libft/libft.h"
#include <fcntl.h>

#define 	WIDTH						1366
#define 	HEIGHT						768

#define 	KB_Esc						53
#define		KB_X						7
#define		KB_Z						6
#define 	KB_Up						126
#define		KB_Down						125
#define 	KB_Left						123
#define		KB_Right					124
#define 	KP_Add                      69
#define		KP_Subtract					78
#define 	KB_i                        34
#define		KB_u						32
#define 	KP_4                        86
#define 	KP_6                        88
#define		KP_8						91
#define		KP_2						84
#define		KB_W						13
#define		KB_S						1

#define		LMB							1
#define		RMB							2
#define		MWD							5
#define		MWU							4

#define		PI							3.14159265359

typedef	struct	s_coords
{
	int					c;
	struct s_coords		*next;
}				t_coords;

typedef struct	s_eulers
{
	float	sin;
	float	cos;
}				t_eulers;

typedef struct	s_point
{
	int			x;
	int			y;
	int			z;
	int			rgb;
}				t_point;

typedef struct	s_color
{
	char		r;
	char		g;
	char		b;
}				t_color;

typedef struct	s_cam
{
	t_point		*pos;
	t_point		*vpoint;
	short		zoom;
	int			xoff;
	int			yoff;
	float		alph;
	float		beta;
	float		gamm;
}				t_cam;

typedef struct	s_mlx
{
	void		*mlx;
	void		*window;
	void		*img;
	char		*pxl;
	int			*coord_arr;
	int			w;
	int			h;
	int			bpp;
	int			s_line;
	int			ed;
	t_cam		*cam;
	int			min_depth;
	int			max_depth;
}				t_mlx;

void	draw(t_mlx *mlx_p);
#endif