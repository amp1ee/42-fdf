#include <mlx.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include "../libft/libft.h"
#include <fcntl.h>

#define 	WIDTH						600
#define 	HEIGHT						400
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

typedef	struct	s_coords
{
	int					c;
	struct s_coords		*next;
}				t_coords;

typedef struct	s_mlx
{
	void		*mlx;
	void		*window;
	void		*img;
	char		*pxl;
	t_coords	*coords;
	int			w;
	int			h;
	int			bpp;
	int			s_line;
	int			ed;

}				t_mlx;

void	*draw(t_mlx *mlx_p);