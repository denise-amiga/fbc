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

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#if defined(HAVE_GL_GL_H)
#include "fb_gfx_gl.h"
#endif

#include "../rtlib/fb.h"
#include "../rtlib/fb_error.h"
#include "../rtlib/fb_scancodes.h"

#ifdef PI
#undef PI
#endif
#define PI			3.1415926535897932384626

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

#ifdef TARGET_X86
 #define RORW(num, bits) __asm__ __volatile__("rorw %1, %0" : "=m"(num) : "c"(bits) : "memory")
 #define RORW1(num)      __asm__ __volatile__("rorw $1, %0" : "=m"(bit) : : "memory");
#else
 #define RORW(num, bits) num = ( (num) >> (bits) ) | (num << (16 - bits) )
 #define RORW1(num)      RORW(num, 1)
#endif

#define BYTES_PER_PIXEL(d)		(((d) + 7) / 8)
#define BPP_MASK(b)				((int)((1LL << ((b) << 3)) - 1))

#define DRIVER_LOCK()		{ if (!(__fb_gfx->flags & (SCREEN_LOCKED | SCREEN_AUTOLOCKED))) { __fb_gfx->driver->lock(); __fb_gfx->flags |= SCREEN_LOCKED | SCREEN_AUTOLOCKED; } }
#define DRIVER_UNLOCK()		{ if (__fb_gfx->flags & SCREEN_AUTOLOCKED) { __fb_gfx->driver->unlock(); __fb_gfx->flags &= ~(SCREEN_LOCKED | SCREEN_AUTOLOCKED); } }
#define SET_DIRTY(c,y,h)	{ if (__fb_gfx->framebuffer == (c)->line[0]) fb_hMemSet(__fb_gfx->dirty + (y), TRUE, (h)); }

#define EVENT_LOCK()		{ fb_MutexLock(__fb_gfx->event_mutex); }
#define EVENT_UNLOCK()		{ fb_MutexUnlock(__fb_gfx->event_mutex); }

#define DRIVER_NULL				-1
#define DRIVER_FULLSCREEN		0x00000001
#define DRIVER_OPENGL			0x00000002
#define DRIVER_NO_SWITCH		0x00000004
#define DRIVER_NO_FRAME			0x00000008
#define DRIVER_SHAPED_WINDOW	0x00000010
#define DRIVER_ALWAYS_ON_TOP	0x00000020
#define DRIVER_ALPHA_PRIMITIVES	0x00000040
#define DRIVER_OPENGL_OPTIONS	0x000F0000
#define HAS_STENCIL_BUFFER		0x00010000
#define HAS_ACCUMULATION_BUFFER	0x00020000
#define HAS_MULTISAMPLE			0x00040000

#define HAS_MMX					0x01000000
#define SCREEN_EXIT				0x80000000
#define SCREEN_LOCKED			0x00000001
#define SCREEN_AUTOLOCKED		0x00000002
#define PRINT_SCROLL_WAS_OFF	0x00000004
#define ALPHA_PRIMITIVES		0x00000008
#define OPENGL_PRIMITIVES		0x00000010
#define QB_COMPATIBILITY		0x10000000
#define OPENGL_SUPPORT			0x20000000

#define CTX_BUFFER_INIT			0x00000001
#define CTX_BUFFER_SET			0x00000002
#define CTX_WINDOW_ACTIVE		0x00000004
#define CTX_WINDOW_SCREEN		0x00000008
#define CTX_VIEWPORT_SET		0x00000010
#define CTX_VIEW_SCREEN			0x00000020

#define COORD_TYPE_AA			0
#define COORD_TYPE_AR			1
#define COORD_TYPE_RA			2
#define COORD_TYPE_RR			3
#define COORD_TYPE_A			4
#define COORD_TYPE_R			5
#define COORD_TYPE_MASK			0x00000007
#define DEFAULT_COLOR_1			0x80000000
#define DEFAULT_COLOR_2			0x40000000
#define VIEW_SCREEN				0x00000001

#define LINE_TYPE_LINE		0
#define LINE_TYPE_B		1
#define LINE_TYPE_BF		2

#define PAINT_TYPE_FILL		0
#define PAINT_TYPE_PATTERN	1

#define MASK_COLOR_32		0xFF00FF
#define MASK_COLOR_16		0xF81F

#define MASK_RB_32			0x00FF00FF
#define MASK_G_32			0x0000FF00
#define MASK_A_32			0xFF000000

#define MASK_RB_16			0xF81F
#define MASK_R_16			0xF800
#define MASK_G_16			0x07E0
#define MASK_B_16			0x001F

#define PUT_HEADER_NEW		0x7

#define PUT_MODE_TRANS		0
#define PUT_MODE_PSET		1
#define PUT_MODE_PRESET		2
#define PUT_MODE_AND		3
#define PUT_MODE_OR			4
#define PUT_MODE_XOR		5
#define PUT_MODE_ALPHA		6
#define PUT_MODE_ADD		7
#define PUT_MODE_CUSTOM		8
#define PUT_MODE_BLEND		9
#define PUT_MODES			10

#define KEY_BUFFER_LEN		16

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

#define WINDOW_TITLE_SIZE	128

#define EVENT_KEY_PRESS				1
#define EVENT_KEY_RELEASE			2
#define EVENT_KEY_REPEAT			3
#define EVENT_MOUSE_MOVE			4
#define EVENT_MOUSE_BUTTON_PRESS	5
#define EVENT_MOUSE_BUTTON_RELEASE	6
#define EVENT_MOUSE_DOUBLE_CLICK	7
#define EVENT_MOUSE_WHEEL			8
#define EVENT_MOUSE_ENTER			9
#define EVENT_MOUSE_EXIT			10
#define EVENT_WINDOW_GOT_FOCUS		11
#define EVENT_WINDOW_LOST_FOCUS		12
#define EVENT_WINDOW_CLOSE			13

#define MAX_EVENTS					128

#define GET_WINDOW_POS				0
#define GET_WINDOW_TITLE			1
#define GET_WINDOW_HANDLE			2
#define GET_DESKTOP_SIZE			3
#define GET_SCREEN_SIZE				4
#define GET_SCREEN_DEPTH			5
#define GET_SCREEN_BPP				6
#define GET_SCREEN_PITCH			7
#define GET_SCREEN_REFRESH			8
#define GET_DRIVER_NAME				9
#define GET_TRANSPARENT_COLOR		10
#define GET_VIEWPORT				11
#define GET_PEN_POS					12
#define GET_COLOR					13
#define GET_ALPHA_PRIMITIVES		14
#define GET_GL_EXTENSIONS			15

#define SET_FIRST_SETTER			100
#define SET_WINDOW_POS				100
#define SET_WINDOW_TITLE			101
#define SET_PEN_POS					102
#define SET_DRIVER_NAME				103
#define SET_ALPHA_PRIMITIVES		104
#define SET_GL_COLOR_BITS			105
#define SET_GL_COLOR_RED_BITS		106
#define SET_GL_COLOR_GREEN_BITS		107
#define SET_GL_COLOR_BLUE_BITS		108
#define SET_GL_COLOR_ALPHA_BITS		109
#define SET_GL_DEPTH_BITS			110
#define SET_GL_STENCIL_BITS			111
#define SET_GL_ACCUM_BITS			112
#define SET_GL_ACCUM_RED_BITS		113
#define SET_GL_ACCUM_GREEN_BITS		114
#define SET_GL_ACCUM_BLUE_BITS		115
#define SET_GL_ACCUM_ALPHA_BITS		116
#define SET_GL_NUM_SAMPLES			117

#define POLL_EVENTS					200


typedef void (BLITTER)(unsigned char *, int);
typedef FBCALL unsigned int (BLENDER)(unsigned int, unsigned int, void *);
typedef void (PUTTER)(unsigned char *, unsigned char *, int, int, int, int, int, BLENDER *, void *);


typedef struct _GFX_CHAR_CELL {
    FB_WCHAR ch;
    unsigned fg, bg;
} GFX_CHAR_CELL;


struct _EVENT {
	int type;
	union {
		struct {			/* keyboard events */
			int scancode;
			int ascii;
		};
		struct {			/* mouse events */
			int x, y;
			int dx, dy;
		};
		int button;
		int z;
	};
} __attribute__((__packed__));
typedef struct _EVENT EVENT;


typedef struct FB_GFXCTX {
	int id;
	int work_page;
	unsigned char **line;
	int max_h;
	int target_bpp;
	int target_pitch;
	void *last_target;
	float last_x, last_y;
	union {
	    struct {
	    	int view_x, view_y, view_w, view_h;
	    };
	    int view[4];
	};
	union {
	    struct {
	    	int old_view_x, old_view_y, old_view_w, old_view_h;
	    };
		int old_view[4];
	};
    float win_x, win_y, win_w, win_h;
	unsigned int fg_color, bg_color;
	void (*put_pixel)(struct FB_GFXCTX *ctx, int x, int y, unsigned int color);
	unsigned int (*get_pixel)(struct FB_GFXCTX *ctx, int x, int y);
	void *(*pixel_set)(void *dest, int color, size_t size);
	PUTTER **putter;
	int put_bpp;
    int flags;
} FB_GFXCTX;


typedef struct FBGFX
{
	int id;									/* Mode id number for contexts identification */
    int mode_num;							/* Current mode number */
    unsigned char **page;					/* Pages memory */
    int num_pages;							/* Number of requested pages */
    int visible_page;						/* Current visible page number */
    unsigned char *framebuffer;				/* Our current visible framebuffer */
    int w, h;								/* Current mode width and height */
    int depth;								/* Current mode depth in bits per pixel */
    int bpp;								/* Bytes per pixel */
    int pitch;								/* Width of a framebuffer line in bytes */
    unsigned int *palette;					/* Current RGB color values for each palette index */
    unsigned int *device_palette;			/* Current RGB color values of visible device palette */
    unsigned char *color_association;		/* Palette color index associations for CGA/EGA emulation */
    char *dirty;							/* Dirty lines buffer */
    const struct GFXDRIVER *driver;			/* Gfx driver in use */
    int color_mask;							/* Color bit mask for colordepth emulation */
    const struct PALETTE *default_palette;	/* Default palette for current mode */
    int scanline_size;						/* Vertical size of a single scanline in pixels */
    int cursor_x, cursor_y;					/* Current graphical text cursor position (in chars, 0 based) */
    const struct FONT *font;				/* Current font */
    int text_w, text_h;						/* Graphical text console size in characters */
    char *key;								/* Keyboard states */
	int refresh_rate;						/* Driver refresh rate */
	GFX_CHAR_CELL **con_pages;				/* Character information for all pages */
    EVENT *event_queue;						/* The OS events queue array */
    int event_head, event_tail;				/* Indices for the head and tail event in the array */
    struct _FBMUTEX *event_mutex;			/* Mutex lock for accessing the events queue */
	volatile int flags;						/* Status flags */
} FBGFX;


typedef struct GFXDRIVER
{
	char *name;
	int (*init)(char *title, int w, int h, int depth, int refresh_rate, int flags);
	void (*exit)(void);
	void (*lock)(void);
	void (*unlock)(void);
	void (*set_palette)(int index, int r, int g, int b);
	void (*wait_vsync)(void);
	int (*get_mouse)(int *x, int *y, int *z, int *buttons);
	void (*set_mouse)(int x, int y, int cursor);
	void (*set_window_title)(char *title);
	int (*set_window_pos)(int x, int y);
	int *(*fetch_modes)(int depth, int *size);
	void (*flip)(void);
	void (*poll_events)(void);
} GFXDRIVER;


typedef struct PALETTE
{
	const int colors;
	const unsigned char *data;
} PALETTE;


typedef struct FONT
{
    const int w;
    const int h;
	const unsigned char *data;
} FONT;


struct _PUT_HEADER {
	union {
		struct {
			unsigned short bpp:3;
			unsigned short width:13;
			unsigned short height;
		} old;
		unsigned int type;
	};
	int bpp;
	unsigned int width;
	unsigned int height;
	unsigned int pitch;
#if defined(HAVE_GL_GL_H)
	GLuint tex;
#else
	unsigned int no_tex;
#endif
	char _reserved[8];
	unsigned char data[0];
} __attribute__((__packed__));
typedef struct _PUT_HEADER PUT_HEADER;


/* Global variables */
extern FBGFX *__fb_gfx;
extern char *__fb_gfx_driver_name;
extern const GFXDRIVER *__fb_gfx_drivers_list[];
extern const GFXDRIVER __fb_gfxDriverNull;
extern void *(*fb_hMemCpy)(void *dest, const void *src, size_t size);
extern void *(*fb_hMemSet)(void *dest, int value, size_t size);
extern void *(*fb_hPixelCpy)(void *dest, const void *src, size_t size);
extern void *(*fb_hPixelSet)(void *dest, int color, size_t size);
extern unsigned int *__fb_color_conv_16to32;
extern char *__fb_window_title;
#if defined(HAVE_GL_GL_H)
extern FB_GL __fb_gl;
extern FB_GL_PARAMS __fb_gl_params;
#endif
#include "fb_gfx_data.h"

/* Internal functions */
extern FB_GFXCTX *fb_hGetContext(void);
extern void fb_hSetupFuncs(int bpp);
extern void fb_hSetupData(void);
extern FBCALL int fb_hEncode(const unsigned char *in_buffer, int in_size, unsigned char *out_buffer, int *out_size);
extern FBCALL int fb_hDecode(const unsigned char *in_buffer, int in_size, unsigned char *out_buffer, int *out_size);
extern void fb_hPostEvent(EVENT *e);
extern void fb_hPostKey(int key);
extern BLITTER *fb_hGetBlitter(int device_depth, int is_rgb);
extern PUTTER *fb_hGetPutter(int mode, int *alpha);
extern unsigned int fb_hMakeColor(int bpp, unsigned int index, int r, int g, int b);
extern unsigned int fb_hFixColor(int bpp, unsigned int color);
extern void fb_hRestorePalette(void);
extern void fb_hPrepareTarget(FB_GFXCTX *ctx, void *target, unsigned int color);
extern void fb_hTranslateCoord(FB_GFXCTX *ctx, float fx, float fy, int *x, int *y);
extern void fb_hFixRelative(FB_GFXCTX *ctx, int coord_type, float *x1, float *y1, float *x2, float *y2);
extern void fb_hFixCoordsOrder(int *x1, int *y1, int *x2, int *y2);
extern void fb_hGfxBox(int x1, int y1, int x2, int y2, unsigned int color, int full, unsigned int style);
extern void fb_hScreenInfo(int *width, int *height, int *depth, int *refresh);
extern void *fb_hMemCpyMMX(void *dest, const void *src, size_t size);
extern void *fb_hMemSetMMX(void *dest, int value, size_t size);
extern void fb_hResetCharCells(FB_GFXCTX *context, int do_alloc);
extern void fb_hClearCharCells(int x1, int y1, int x2, int y2, int page, FB_WCHAR ch, unsigned fg, unsigned bg);
extern void fb_hSoftCursorInit(void);
extern void fb_hSoftCursorExit(void);
extern void fb_hSoftCursorPut(int x, int y);
extern void fb_hSoftCursorUnput(int x, int y);
extern void fb_hSoftCursorPaletteChanged(void);
extern int fb_hColorDistance(int index, int r, int g, int b);
extern void *fb_hPixelSetAlpha4(void *dest, int color, size_t size);
extern int fb_hGetWindowHandle(void);
extern void fb_hGL_NormalizeParameters(int gl_options);
extern int fb_hGL_Init(FB_DYLIB lib, char *os_extensions);
extern int fb_hGL_ExtensionSupported(const char *extension);
extern void *fb_hGL_GetProcAddress(const char *proc);


/* Public API */
extern FBCALL int fb_GfxScreen(int mode, int depth, int num_pages, int flags, int refresh_rate);
extern FBCALL int fb_GfxScreenRes(int width, int height, int depth, int num_pages, int flags, int refresh_rate);
extern FBCALL void fb_GfxScreenInfo(int *width, int *height, int *depth, int *bpp, int *pitch, int *refresh_rate, FBSTRING *driver);
extern FBCALL void *fb_GfxImageCreate(int width, int height, unsigned int color, int depth, int flags);
extern FBCALL void fb_GfxImageDestroy(void *image);
extern FBCALL void fb_GfxPalette(int index, int r, int g, int b);
extern FBCALL void fb_GfxPaletteUsing(int *data);
extern FBCALL void fb_GfxPaletteGet(int index, int *r, int *g, int *b);
extern FBCALL void fb_GfxPaletteGetUsing(int *data);
extern FBCALL void fb_GfxPset(void *target, float x, float y, unsigned int color, int coord_type, int ispreset);
extern FBCALL int fb_GfxPoint(void *target, float x, float y);
extern FBCALL float fb_GfxPMap(float coord, int func);
extern FBCALL float fb_GfxCursor(int func);
extern FBCALL void fb_GfxView(int x1, int y1, int x2, int y2, unsigned int fill_color, unsigned int border_color, int screen);
extern FBCALL void fb_GfxWindow(float x1, float y1, float x2, float y2, int screen);
extern FBCALL void fb_GfxLine(void *target, float x1, float y1, float x2, float y2, unsigned int color, int type, unsigned int style, int coord_type);
extern FBCALL void fb_GfxEllipse(void *target, float x, float y, float radius, unsigned int color, float aspect, float start, float end, int fill, int coord_type);
extern FBCALL int fb_GfxGet(void *target, float x1, float y1, float x2, float y2, unsigned char *dest, int coord_type, FBARRAY *array);
extern FBCALL int fb_GfxPut(void *target, float x, float y, unsigned char *src, int x1, int y1, int x2, int y2, int coord_type, int mode, int alpha, BLENDER *blender, void *param);
extern FBCALL int fb_GfxWaitVSync(void);
extern FBCALL void fb_GfxPaint(void *target, float fx, float fy, unsigned int color, unsigned int border_color, FBSTRING *pattern, int mode, int coord_type);
extern FBCALL void fb_GfxDraw(void *target, FBSTRING *command);
extern FBCALL int fb_GfxDrawString(void *target, float fx, float fy, int coord_type, FBSTRING *string, unsigned int color, void *font, int mode, BLENDER *blender, void *param);
extern FBCALL void fb_GfxFlip(int from_page, int to_page);
extern FBCALL void fb_GfxSetPage(int work_page, int visible_page);
extern FBCALL void fb_GfxLock(void);
extern FBCALL void fb_GfxUnlock(int start_line, int end_line);
extern FBCALL void *fb_GfxScreenPtr(void);
extern FBCALL void fb_GfxSetWindowTitle(FBSTRING *title);
extern FBCALL int fb_GfxGetJoystick(int id, int *buttons, float *a1, float *a2, float *a3, float *a4, float *a5, float *a6, float *a7, float *a8);
extern FBCALL int fb_GfxEvent(EVENT *event);
extern FBCALL void fb_GfxControl_s(int what, FBSTRING *param);
extern FBCALL void fb_GfxControl_i(int what, int *param1, int *param2, int *param3, int *param4);
extern FBCALL int fb_GfxBload(FBSTRING *filename, void *dest, void *pal);
extern FBCALL int fb_GfxBsave(FBSTRING *filename, void *src, unsigned int size, void *pal);
extern FBCALL void *fb_GfxGetGLProcAddress(const char *proc);


/* Runtime library hooks */
int fb_GfxGetkey(void);
FBSTRING *fb_GfxInkey(void);
int fb_GfxKeyHit(void);
int fb_GfxColor(int fg_color, int bg_color, int flags);
void fb_GfxClear(int mode);
int fb_GfxWidth(int w, int h);
int fb_GfxLocateRaw(int y, int x, int cursor);
int fb_GfxLocate(int y, int x, int cursor);
int fb_GfxGetX(void);
int fb_GfxGetY(void);
void fb_GfxGetXY(int *col, int *row);
void fb_GfxGetSize(int *cols, int *rows);
void fb_GfxPrintBuffer(const char *buffer, int mask);
void fb_GfxPrintBufferWstr(const FB_WCHAR *buffer, int mask);
void fb_GfxPrintBufferEx(const void *buffer, size_t len, int mask);
void fb_GfxPrintBufferWstrEx(const FB_WCHAR *buffer, size_t len, int mask);
char *fb_GfxReadStr(char *buffer, int maxlen);
int fb_GfxMultikey(int scancode);
int fb_GfxGetMouse(int *x, int *y, int *z, int *buttons);
int fb_GfxSetMouse(int x, int y, int cursor);
int fb_GfxOut(unsigned short port, unsigned char value);
int fb_GfxIn(unsigned short port);
int fb_GfxLineInput( FBSTRING *text, void *dst, int dst_len, int fillrem, int addquestion, int addnewline );
int fb_GfxLineInputWstr( const FB_WCHAR *text, FB_WCHAR *dst, int max_chars, int addquestion, int addnewline );
unsigned int fb_GfxReadXY( int col, int row, int colorflag );
void fb_GfxSleep( int msecs );
int fb_GfxIsRedir( int is_input );

typedef void (*FBGFX_IMAGE_CONVERT)(const unsigned char *, unsigned char *, int);

void fb_image_convert_8to8(const unsigned char *src, unsigned char *dest, int w);
void fb_image_convert_8to16(const unsigned char *src, unsigned char *dest, int w);
void fb_image_convert_8to32(const unsigned char *src, unsigned char *dest, int w);
void fb_image_convert_24to16(const unsigned char *src, unsigned char *dest, int w);
void fb_image_convert_24to32(const unsigned char *src, unsigned char *dest, int w);
void fb_image_convert_32to16(const unsigned char *src, unsigned char *dest, int w);
void fb_image_convert_32to32(const unsigned char *src, unsigned char *dest, int w);
void fb_image_convert_24bgrto16(const unsigned char *src, unsigned char *dest, int w);
void fb_image_convert_24bgrto32(const unsigned char *src, unsigned char *dest, int w);
void fb_image_convert_32bgrto16(const unsigned char *src, unsigned char *dest, int w);
void fb_image_convert_32bgrto32(const unsigned char *src, unsigned char *dest, int w);

FBCALL void fb_GfxImageConvertRow( const unsigned char *src, int src_bpp, unsigned char *dest, int dst_bpp, int width, int isrgb );

/** Returns TRUE if application is in graphics mode.
 */
#define FB_GFX_ACTIVE() \
    (__fb_gfx!=NULL)

/** Returns the code page as integral value.
 *
 * This function returns the code page as integral value. When the code page
 * cannot be expressed as an integral value (like UTF-8 or UCS-4), it returns
 * -1 and the character set ID should be used instead.
 */
#define FB_GFX_GET_CODEPAGE() \
    437

/** Returns the character set as a string.
 */
#define FB_GFX_GET_CHARSET() \
    "CP437"

#ifdef __cplusplus
}
#endif

#endif
