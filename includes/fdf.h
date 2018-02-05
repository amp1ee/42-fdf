/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/04 16:52:47 by oahieiev          #+#    #+#             */
/*   Updated: 2018/02/04 16:52:48 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

#include <stdio.h> //--------------------DELETE
# include "../libft/libft.h"

# define WIDTH 1280
# define HEIGHT 800

# ifdef __APPLE__
#  include "keyb_mac.h"
# elif __linux__
#  include "keyb_linux.h"
# endif

# define LMB 1
# define MWD 5
# define MWU 4

# define INIT_ERR_MSG "ERROR: Couldn't initialize t_fdf struct"
# define READ_ERR_MSG "ERROR: Couldn't read the specified map file"
# define USG_ERR_MSG "Usage: ./fdf MAP_FILE"

# define TXT_COLOR 0xCCCCCC
# define PI 3.14159265359
# define MIN(x, y) x < y ? x : y

typedef struct			s_point
{
	int					x;
	int					y;
	int					z;
	int					rgb;
}						t_point;

typedef struct			s_cam
{
	short				zoom;
	char				isom;
	int					xoff;
	int					yoff;
	float				alph;
	float				beta;
	float				gamm;
	float				zdiv;
}						t_cam;

typedef struct			s_map
{
	int					w;
	int					h;
	int					*coord_arr;
	int					*color_arr;
	int					min_z;
	int					max_z;
	int					rng;
	unsigned char		cm;
}						t_map;

typedef struct			s_fdf
{
	void				*mlx;
	void				*window;
	void				*img;
	char				*pxl;
	int					bpp;
	int					s_line;
	int					endian;
	t_cam				*cam;
	t_map				*map;
}						t_fdf;

t_map					*read_map(int fd);
void					draw(t_fdf *fdf, int w, int h);
double					find_perc(double start, double end, double cur);
int						interp_color(int c1, int c2, double p);
int						get_color(int z, t_map map);
int						mouse_pressed(int button, int mousex, int mousey,
	void *param);
int						key_pressed(int keycode, void *param);
float					rad(float deg);
t_point					assign_point(int x, int y);
int						ind(int x, int y, int w, int h);
int						terminate(char *err);
void					*cleanall(t_fdf *fdf);
#endif
