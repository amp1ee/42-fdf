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

#ifdef		__APPLE__
	#include "keyb_mac.h"
#elif		__linux__
	#include "keyb_linux.h"
#endif

#define		LMB							1
#define		MWD							5
#define		MWU							4

#define		INIT_ERR_MSG				"ERROR: couldn't initialize t_mlx struct"
#define		READ_ERR_MSG				"ERROR: couldn't read the specified map file"
#define		USG_ERR_MSG					"Usage: ./fdf <map.fdf>"

#define		PI							3.14159265359
#define		MIN(x, y)					x < y ? x : y

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

typedef struct	s_map
{
	int			w;
	int			h;
	int			*coord_arr;
	int			min_depth;
	int			max_depth;
}				t_map;

typedef struct	s_mlx
{
	void		*mlx;
	void		*window;
	void		*img;
	char		*pxl;
	int			bpp;
	int			s_line;
	int			endian;
	t_cam		*cam;
	t_map		*map;
}				t_mlx;

void		draw(t_mlx *mlx_p);
int			mouse_pressed(int button, int mousex, int mousey, void *param);
int			key_handler(int keycode, void *param);
float		rad(float deg);
t_map		*read_map(int fd, char **line, t_coords **coords);
void		draw(t_mlx *fdf);
int			ind(int x, int y, int w, int h);
t_eulers	get_eulers(float angle);
t_point		assign_point(int x, int y);
double		find_perc(double start, double end, double cur);
int			interp_color(int c1, int c2, double p);
int			get_color(int z, t_mlx m);
#endif
