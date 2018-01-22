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
#define 	KB_Esc						0xff1b
#define		KB_X						120
#define		KB_Z						122
#define 	KB_Up						65362
#define		KB_Down						65364
#define 	KB_Left						65361
#define		KB_Right					65363
#define 	KP_Add                      0xffab
#define		KP_Subtract					0xffad
#define 	KB_i                        0x0069
#define		KB_u						0x0075
#define 	KP_4                        0xff96
#define 	KP_6                        0xff98
#define		KP_8						0xff97
#define		KP_2						0xff99
#define		KB_W						119
#define		KB_S						115

#define		PI							3.14159265359

typedef	struct	s_coords
{
	int					c;
	struct s_coords		*next;
}				t_coords;

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

void	*draw(t_mlx *mlx_p);
#endif