#include "keine.h"

#ifndef MOKOU_H
#define MOKOU_H

INLINE u32 mokou_sprpos(s32 x,s32 y)
{ return ((u16)(x)<<16) | (u16)(y); }
// pos split into 2 halfwords,XY
// flip is 2 bits, XY
typedef struct mokou_sprattr { u32 pos; u8 flip; u16 fillp; FIXED sx,sy; } mokou_sprattr;

/*	--	pset functions	--	*/
extern RGB8 *mokou_pread4(keine* yago,u32 x, u32 y);
extern void mokou_pset4(keine* yago,s32 x, s32 y,RGB8 c);
extern RGB8 mokou_pget4(keine* yago,s32 x, s32 y);

extern RGB8 *mokou_pread8(keine* yago,u32 x, u32 y);
extern void mokou_pset8(keine* yago,s32 x, s32 y,RGB8 c);
extern RGB8 mokou_pget8(keine* yago,s32 x, s32 y);

extern RGB16 *mokou_pread16(keine* yago,u32 x, u32 y);
extern void mokou_pset16(keine* yago,s32 x, s32 y,RGB16 c);
extern RGB16 mokou_pget16(keine* yago,s32 x, s32 y);

/*	--	fill functions	--	*/
extern void mokou_spr16(
	keine *src,keine *dst,SDL_Rect srcrect,
	mokou_sprattr attr;
);

extern void mokou_trifillbot(keine *yago,VEC2 *v,s32 c);
extern void mokou_trifilltop(keine *yago,VEC2 *v,s32 c);

#endif
