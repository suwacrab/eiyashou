#include "kbase.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#ifndef EIRIN_H
#define EIRIN_H

// rgb inlines & defs
INLINE u16 RGB565(u32 r,u32 g,u32 b) { return (r<<11) | (g<<5) | b; }
INLINE u16 BGR565(u32 r,u32 g,u32 b) { return (b<<11) | (g<<5) | r; }

typedef u16 RGB16;

/*	--	enums	--	*/
typedef enum eirin_pixelfmt
{
	EIRIN_PIXELFMT_PAL4, // 16-color palette, first pixel == first nybble
	EIRIN_PIXELFMT_PAL8, // 256-color palette, each pixel == 1 byte
	EIRIN_PIXELFMT_RGB565, // 16-bit high clr (BBBBBGGGGGGRRRRR in bits)
} eirin_pixelfmt;

/*	--	structs	--	*/
typedef struct eirin
{
	u32 w,h; // width,height
	eirin_pixelfmt fmt; // pixel format
	void *palette; // color palette
	void *m; // pixel matrix
} eirin;

typedef void eirin_initfunc(eirin*);

/*	--	main functions	--	*/
extern eirin *eirin_init(eirin *yago,u32 w,u32 h,eirin_pixelfmt fmt);
extern eirin *eirin_loadimg(eirin *yago,const char *fname,eirin_pixelfmt fmt);
extern eirin *eirin_freeimg(eirin *yago);

/*	--	init functions	--	*/
extern eirin_initfunc *eirin_initmodes[];
extern void eirin_initPAL4(eirin *yago);
extern void eirin_initPAL8(eirin *yago);
extern void eirin_initRGB565(eirin *yago);

#endif
