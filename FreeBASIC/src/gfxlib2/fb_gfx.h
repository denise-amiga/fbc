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
 * fb_gfx.h -- internal gfx definitions
 *
 * chng: jan/2005 written [lillo]
 *
 */

#ifndef __FB_GFX_H__
#define __FB_GFX_H__

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../rtlib/fb.h"

#ifndef TRUE
#define TRUE	-1
#endif
#ifndef FALSE
#define FALSE	0
#endif
#ifdef PI
#undef PI
#endif
#define PI			3.141593f
#ifdef MIN
#undef MIN
#endif
#define MIN(a,b)		((a) < (b) ? (a) : (b))
#ifdef MAX
#undef MAX
#endif
#define MAX(a,b)		((a) > (b) ? (a) : (b))
#ifdef MID
#undef MID
#endif
#define MID(a,b,c)		MIN(MAX((a), (b)), (c))
#ifdef SWAP
#undef SWAP
#endif
#define SWAP(a,b)		((a) ^= (b), (b) ^= (a), (a) ^= (b))


#define DRIVER_FULLSCREEN	0x00000001

#define HAS_MMX			0x01000000
#define SCREEN_EXIT		0x80000000
#define DEFAULT_COLOR		0xFFFF0000
#define WINDOW_ACTIVE		0x00000001
#define WINDOW_SCREEN		0x00000002
#define VIEW_SCREEN		0x00000004

#define COORD_TYPE_AA		0
#define COORD_TYPE_AR		1
#define COORD_TYPE_RA		2
#define COORD_TYPE_RR		3
#define COORD_TYPE_A		4
#define COORD_TYPE_R		5

#define LINE_TYPE_LINE		0
#define LINE_TYPE_B		1
#define LINE_TYPE_BF		2

#define PAINT_TYPE_FILL		0
#define PAINT_TYPE_PATTERN	1

#define MASK_COLOR_32		0xFF00FF
#define MASK_COLOR_16		0xF81F

#define PUT_MODE_TRANS		0
#define PUT_MODE_PSET		1
#define PUT_MODE_PRESET		2
#define PUT_MODE_AND		3
#define PUT_MODE_OR		4
#define PUT_MODE_XOR		5

#define KEY_QUIT		0x100
#define KEY_UP			0x101
#define KEY_DOWN		0x102
#define KEY_LEFT		0x103
#define KEY_RIGHT		0x104
#define KEY_INS			0x105
#define KEY_DEL			0x106
#define KEY_HOME		0x107
#define KEY_END			0x108
#define KEY_PAGE_UP		0x109
#define KEY_PAGE_DOWN		0x10A
#define KEY_F(n)		(0x10A + (n))
#define KEY_MAX_SPECIALS	(KEY_F(10) - 0x100 + 1)



typedef struct MODE
{
	unsigned char **page;				/* Pages memory */
	int num_pages;					/* Number of requested pages */
	unsigned char *framebuffer;			/* Our current visible framebuffer */
	unsigned char **line;				/* Line pointers into current active framebuffer */
	int pitch;					/* Width of a framebuffer line in bytes */
	int bpp;					/* Bytes per pixel */
	unsigned int *palette;				/* Current RGB color values for each palette index */
	char *dirty;					/* Dirty lines buffer */
	const struct GFXDRIVER *driver;			/* Gfx driver in use */
	int w, h;					/* Current mode width and height */
	int depth;					/* Current mode depth */
	int color_mask;					/* Color bit mask for colordepth emulation */
	const unsigned char *default_palette;		/* Default palette data for current mode */
	int scanline_size;				/* Vertical size of a single scanline in pixels */
	int fg_color, bg_color;				/* Current foreground and background colors */
	float last_x, last_y;				/* Last pen position */
	int cursor_x, cursor_y;				/* Current graphical text cursor position (in chars, 0 based) */
	const struct FONT *font;			/* Current font */
	int view_x, view_y, view_w, view_h;		/* VIEW coordinates */
	float win_x, win_y, win_w, win_h;		/* WINDOW coordinates */
	int text_w, text_h;				/* Graphical text console size in characters */
	void *scratch_mem;				/* Scratch memory */
	int scratch_mem_ptr;				/* Current free pointer in scratch memory block */
	int scratch_mem_size;				/* Size of scratch memory block */
	char *key;					/* Keyboard states */
	int flags;					/* Status flags */
} MODE;


typedef struct GFXDRIVER
{
	int (*init)(char *title, int w, int h, int depth, int flags);
	void (*exit)(void);
	void (*lock)(void);
	void (*unlock)(void);
	void (*set_palette)(int index, int r, int g, int b);
	void (*wait_vsync)(void);
	int (*get_key)(int wait);
} GFXDRIVER;


typedef struct FONT
{
	const int h;
	const unsigned char *data;
} FONT;


typedef void (BLITTER)(unsigned char *, int);

/* Global variables */
extern MODE *fb_mode;
extern void *(*fb_hMemCpy)(void *dest, const void *src, size_t size);
extern void *(*fb_hMemSet)(void *dest, int value, size_t size);
extern void (*fb_hPutPixel)(int x, int y, unsigned int color);
extern unsigned int (*fb_hGetPixel)(int x, int y);
extern void *(*fb_hPixelCpy)(void *dest, const void *src, size_t size);
extern void *(*fb_hPixelSet)(void *dest, int color, size_t size);
extern const unsigned char fb_cga_palette[12][3];
extern const unsigned char fb_ega_palette[80][3];
extern const unsigned char fb_vga_palette[256][3];
extern const FONT fb_font_8x8;
extern const FONT fb_font_8x14;
extern const FONT fb_font_8x16;
#ifdef WIN32
extern GFXDRIVER fb_gfxDriverDirectDraw;
#endif


/* Internal functions */
extern void fb_hSetupFuncs(void);
extern void fb_hMemInit(int size);
extern void *fb_hMemAlloc(int size);
extern BLITTER *fb_hGetBlitter(int device_depth, int is_rgb);
extern unsigned int fb_hMakeColor(int index, int r, int g, int b);
extern unsigned int fb_hFixColor(unsigned int color);
extern void fb_hTranslateCoord(float fx, float fy, int *x, int *y);
extern void fb_hFixRelative(int coord_type, float *x1, float *y1, float *x2, float *y2);
extern void fb_hFixCoordsOrder(int *x1, int *y1, int *x2, int *y2);
extern void fb_hGfxBox(int x1, int y1, int x2, int y2, int color, int full);
extern int fb_hHasMMX(void);
extern void *fb_hMemCpyMMX(void *dest, const void *src, size_t size);
extern void *fb_hMemSetMMX(void *dest, int value, size_t size);

/* Public API */
extern FBCALL int fb_GfxScreen(int mode, int depth, int num_pages, int flags);
extern FBCALL void fb_GfxPalette(int index, unsigned int color);
extern FBCALL void fb_GfxPaletteOut(int port, int value);
extern FBCALL int fb_GfxPaletteIn(int port);
extern FBCALL int fb_GfxRgb(unsigned char r, unsigned char g, unsigned char b);
extern FBCALL void fb_GfxPset(float x, float y, int color, int coord_type);
extern FBCALL int fb_GfxPoint(float x, float y);
extern FBCALL float fb_GfxPMap(float coord, int func);
extern FBCALL float fb_GfxCursor(int func);
extern FBCALL void fb_GfxView(int x1, int y1, int x2, int y2, int fill_color, int border_color, int screen);
extern FBCALL void fb_GfxWindow(float x1, float y1, float x2, float y2, int screen);
extern FBCALL void fb_GfxLine(float x1, float y1, float x2, float y2, int color, int type, unsigned int style, int coord_type);
extern FBCALL void fb_GfxEllipse(float x, float y, float radius, int color, float aspect, float start, float end, int fill, int coord_type);
extern FBCALL void fb_GfxGet(float x1, float y1, float x2, float y2, unsigned char *dest, int coord_type, FBARRAY *array);
extern FBCALL void fb_GfxPut(float x, float y, unsigned char *src, int coord_type, int mode);
extern FBCALL void fb_GfxWaitVSync(int port, int and_mask, int xor_mask);
extern FBCALL void fb_GfxPaint(float fx, float fy, int color, int border_color, FBSTRING *pattern, int mode, int coord_type);
extern FBCALL void fb_GfxFlip(int from_page, int to_page);
extern FBCALL void fb_GfxSetPage(int work_page, int visible_page);
extern FBCALL void fb_GfxLock(void);
extern FBCALL void fb_GfxUnlock(void);
extern FBCALL void *fb_GfxScreenPtr(void);

/* Runtime library hooks */
int fb_GfxGetkey(void);
FBSTRING *fb_GfxInkey(void);
void fb_GfxColor(int fg_color, int bg_color);
void fb_GfxClear(int mode);
void fb_GfxWidth(int w, int h);
void fb_GfxLocate(int y, int x, int cursor);
int fb_GfxGetX(void);
int fb_GfxGetY(void);
void fb_GfxPrintBuffer(char *buffer, int mask);

#endif
