/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/04 16:41:29 by oahieiev          #+#    #+#             */
/*   Updated: 2018/02/04 16:41:31 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <mlx.h>

int				mouse_pressed(int button, int mousex, int mousey, void *param)
{
	t_fdf			*fdf;

	fdf = (t_fdf *)param;
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
	draw(fdf, (*(*fdf).map).w, (*(*fdf).map).h);
	return (0);
}

static void		switch_view(t_fdf *fdf)
{
	fdf->cam->yoff -= (fdf->map->w * fdf->cam->zoom / 2) *
		((fdf->cam->isom) ? 1 : -1);
	fdf->cam->alph = fdf->cam->isom ? 0 : rad(-35.264);
	fdf->cam->beta = 0;
	fdf->cam->gamm = fdf->cam->isom ? 0 : rad(45);
	fdf->cam->isom = !(fdf->cam->isom);
}

static void		move(int key, t_fdf *p)
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

static void		rotate(int key, t_fdf *fdf)
{
	if (key == KB_UP)
		fdf->cam->alph += rad(2);
	else if (key == KB_DOWN)
		fdf->cam->alph -= rad(2);
	else if (key == KB_LEFT)
		fdf->cam->gamm += rad(2);
	else if (key == KB_RIGHT)
		fdf->cam->gamm -= rad(2);
	else if (key == KB_W)
		fdf->cam->beta -= rad(2);
	else
		fdf->cam->beta += rad(2);
}

int				key_pressed(int key, void *param)
{
	t_fdf	*fdf;

	fdf = (t_fdf *)param;
	if (key == KB_ESC)
	{
		cleanall(fdf);
		exit(0);
		return (0);
	}
	else if (key == KB_UP || key == KB_DOWN || key == KB_LEFT ||
		key == KB_RIGHT || key == KB_W || key == KB_S)
		rotate(key, fdf);
	else if (key == KP_4 || key == KP_6 || key == KP_8 || key == KP_2)
		move(key, fdf);
	else if (key == KB_I)
		switch_view(fdf);
	else if (key == KP_SUBTRACT)
		fdf->cam->zdiv += 0.333;
	else if (key == KP_ADD && fdf->cam->zdiv >= 1.3)
		fdf->cam->zdiv -= 0.333;
	else if (key == KB_U)
		fdf->map->cm += 5;
	draw(fdf, (*(*fdf).map).w, (*(*fdf).map).h);
	return (0);
}
