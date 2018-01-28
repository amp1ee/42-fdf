#include "fdf.h"
#include <mlx.h>

int				mouse_pressed(int button, int mousex, int mousey, void *param)
{
	t_mlx			*fdf;

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
		fdf->cam->zoom++;
	if (fdf->cam->zoom == 0)
		fdf->cam->zoom = 1;
	draw(fdf);
	return (0);
}

static void		switch_view(t_mlx *p)
{
	p->cam->yoff -= (p->map->w * p->cam->zoom / 2) *
		((p->cam->isom) ? 1 : -1);
	p->cam->alph = p->cam->isom ? 0 : rad(-35.264);
	p->cam->beta = 0;
	p->cam->gamm = p->cam->isom ? 0 : rad(45);
	p->cam->isom = !p->cam->isom;
}

static void		move(int key, t_mlx *p)
{
	if (key == KP_4)
		p->cam->xoff += 10;
	else if (key == KP_6)
		p->cam->xoff -= 10;
	else if (key == KP_8)
		p->cam->yoff += 10;
	else
		p->cam->yoff -= 10;
}

static void		rotate(int key, t_mlx *p)
{
	if (key == KB_Up)
		p->cam->alph += rad(2);
	else if (key == KB_Down)
		p->cam->alph -= rad(2);
	else if (key == KB_Left)
		p->cam->gamm += rad(2);
	else if (key == KB_Right)
		p->cam->gamm -= rad(2);
	else if (key == KB_W)
		p->cam->beta -= rad(2);
	else
		p->cam->beta += rad(2);
}

int				key_pressed(int key, void *param)
{
	t_mlx	*p;

	p = (t_mlx *)param;
	if (key == KB_Esc)
	{
		cleanall(p);
		exit(0);
		return (0);
	}
	else if (key == KB_Up || key == KB_Down || key == KB_Left ||
		key == KB_Right || key == KB_W || key == KB_S)
		rotate(key, p);
	else if (key == KP_4 || key == KP_6 || key == KP_8 || key == KP_2)
		move(key, p);
	else if (key == KB_i)
		switch_view(p);
	else if (key == KP_Add)
		p->cam->zdiv++;
	else if (key == KP_Subtract && p->cam->zdiv != 1)
		p->cam->zdiv--;
	draw(p);
	return (0);
}
