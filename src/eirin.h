#include "kbase.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#ifndef EIRIN_H
#define EIRIN_H

// quick inlines & defs
INLINE u16 RGB565(u32 r,u32 g,u32 b) { return (r<<11) | (g<<5) | b; }
INLINE u16 BGR565(u32 r,u32 g,u32 b) { return (b<<11) | (g<<5) | r; }

typedef u16 RGB16;
typedef u8 RGB8;

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
	u16 fillp; // fill pattern
	eirin_pixelfmt fmt; // pixel format
	void *palette; // color palette
	void *m; // pixel matrix
} eirin;

/*	--	main functions	--	*/
extern eirin *eirin_init(eirin *yago,u32 w,u32 h,eirin_pixelfmt fmt);
extern eirin *eirin_free(eirin *yago);
extern eirin *eirin_loadimg(eirin *yago,const char *fname,eirin_pixelfmt fmt);

/*	--	fill functions	--	*/
extern void eirin_trifillbot(eirin *yago,VEC2 *v,u32 c);
extern void eirin_trifilltop(eirin *yago,VEC2 *v,u32 c);

/*	--	init functions	--	*/
typedef size_t (*eirin_initfunc)(eirin*);
extern eirin_initfunc eirin_initfmts[];
extern size_t eirin_initPAL4(eirin *yago);
extern size_t eirin_initPAL8(eirin *yago);
extern size_t eirin_initRGB565(eirin *yago);

/*	--	inline functions	--	*/
INLINE RGB16 *eirin_pread16(eirin *yago,u32 x,u32 y)
{ return ((u16*)yago->m) + (x + (y*yago->w)); }
INLINE RGB16 eirin_pget16(eirin *yago,u32 x,u32 y)
{ return *eirin_pread16(yago,x,y); }
INLINE void eirin_pset16(eirin *yago,s32 x,s32 y,RGB16 c)
{
	u32 fillpix = (yago->fillp >> ((x&3) + (y*4))) & 1;
	RGB16 clr = (fillpix*c) + ((~fillpix)*eirin_pget16(yago,x,y));
	if(in_range(x,0,yago->w) && in_range(y,0,yago->h))
		(*eirin_pread16(yago,x,y)) = clr; 
}

INLINE size_t eirin_imgsize(u32 w,u32 h,eirin_pixelfmt fmt)
{ return sizeof(u8) * ((w<<1)>>(2-fmt)); }

#endif
