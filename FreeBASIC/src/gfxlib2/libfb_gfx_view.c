/*
 *  libgfx2 - FreeBASIC's alternative gfx library
 *	Copyright (C) 2005 Angelo Mottola (a.mottola@libero.it)
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
 * view.c -- VIEW statement
 *
 * chng: jan/2005 written [lillo]
 *
 */

#include "fb_gfx.h"


/*:::::*/
FBCALL void fb_GfxView(int x1, int y1, int x2, int y2, int fill_color, int border_color, int screen)
{
	if (!fb_mode)
		return;
	
	fb_hFixCoordsOrder(&x1, &y1, &x2, &y2);
	
	fb_mode->view_x = fb_mode->view_y = 0;
	fb_mode->view_w = fb_mode->w;
	fb_mode->view_h = fb_mode->h;
	
	if (border_color != DEFAULT_COLOR)
		fb_hGfxBox(x1 - 1, y1 - 1, x2 + 1, y2 + 1, border_color & fb_mode->color_mask, FALSE);
	
	if ((x1 | y1 | x2 | y2) != 0xFFFF8000) {
		fb_mode->view_x = MID(0, x1, fb_mode->w);
		fb_mode->view_y = MID(0, y1, fb_mode->h);
		fb_mode->view_w = MIN(x2 - x1 + 1, fb_mode->w - x1);
		fb_mode->view_h = MIN(y2 - y1 + 1, fb_mode->h - y1);
	}
	if (screen)
		fb_mode->flags |= VIEW_SCREEN;
	else
		fb_mode->flags &= ~VIEW_SCREEN;
	
	if (fill_color != DEFAULT_COLOR)
		fb_GfxClear(0);
}
