#ifndef FDF_H
#define FDF_H

#include <mlx.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include "../libft/libft.h"
#include <fcntl.h>

#define 	WIDTH						1000
#define 	HEIGHT						600

#ifdef		__APPLE__
	#include "keyb_mac.h"
#elif		__linux__
	#include "keyb_linux.h"
#endif

#define		LMB							1
#define		MWD							5
#define		MWU							4

#define		INIT_ERR_MSG				"FATAL ERROR: couldn't initialize t_mlx struct"

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
	short		zoom;
	char		isom;

	int			xoff;
	int			yoff;

	float		alph;
	float		beta;
	float		gamm;

	short		zdiv;
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
