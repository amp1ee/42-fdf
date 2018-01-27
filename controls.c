#include "fdf.h"

int		mouse_pressed(int button, int mousex, int mousey, void *param)
{
	t_mlx				*fdf;

	fdf = (t_mlx *)param;
	(void)mousex;
	if (mousey < 0)
		return (1);
	else if (button == LMB)
	{
		fdf->cam->xoff = mousex - WIDTH / 2;
		fdf->cam->yoff = mousey - HEIGHT / 2;
	}
	else if (button == MWD)
		fdf->cam->zoom--;
	else if (button == MWU)
	{
		fdf->cam->zoom++;
	}
	if (fdf->cam->zoom == 0)
		fdf->cam->zoom = 1;
	draw(fdf);
	return (0);
}

int		key_handler(int keycode, void *param)
{
	t_mlx	*p;

	p = (t_mlx *)param;
	if (keycode == KB_Esc)
	{
		mlx_destroy_window(p->mlx, p->window);
		ft_memdel((void **)&p);
		exit(0);
		return (0);
	}
	else if (keycode == KB_Up)
		p->cam->alph += rad(2);
	else if (keycode == KB_Down)
		p->cam->alph -= rad(2);
	else if (keycode == KB_Left)
		p->cam->gamm += rad(2);
	else if (keycode == KB_Right)
		p->cam->gamm -= rad(2);
	else if (keycode == KB_W)
		p->cam->beta -= rad(2);
	else if (keycode == KB_S)
		p->cam->beta += rad(2);
	else if (keycode == KP_4)
		p->cam->xoff += 10;
	else if (keycode == KP_6)
		p->cam->xoff -= 10;
	else if (keycode == KP_8)
		p->cam->yoff += 10;
	else if (keycode == KP_2)
		p->cam->yoff -= 10;
	else if (keycode == KB_i)
	{
		p->cam->yoff -= (p->map->w * p->cam->zoom / 2) *
			((p->cam->isom) ? 1 : -1);
		p->cam->alph = p->cam->isom ? 0 : rad(-35.264);
		p->cam->beta = 0;
		p->cam->gamm = p->cam->isom ? 0 : rad(45);
		p->cam->isom = !p->cam->isom;
	}
	else if (keycode == KP_Add)
		p->cam->zdiv++;
	else if (keycode == KP_Subtract)
		p->cam->zdiv--;
	if (p->cam->zdiv == 0)
		p->cam->zdiv = 1;
	if (p->cam->zoom == 0)
		p->cam->zoom = 1;
	draw(p);
	return (0);
}
