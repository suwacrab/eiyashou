#include "keine.h"

#ifndef MOKOU_H
#define MOKOU_H

INLINE u32 mokou_sprpos(s16 x,s16 y)
{
	return ((u16)(x)<<8) | (u16)(y);
}
typedef struct mokou_sprattr { u32 pos; u8 flip; } mokou_sprattr;

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
