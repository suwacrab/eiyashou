#include "mokou.h"

/*	--	pset functions	--	*/
RGB16 *mokou_pread16(keine *yago,u32 x,u32 y)
{ return ((RGB16*)yago->m) + (x + (y*yago->w)); }
RGB16 mokou_pget16(keine *yago,s32 x,s32 y)
{ return keine_clip(yago,x,y) ? (*mokou_pread16(yago,x,y)) : 0; }
void mokou_pset16(keine *yago,s32 x,s32 y,RGB16 c)
{
	if(keine_clip(yago,x,y)) {
		RGB16 *pix = mokou_pread16(yago,x,y);
		RGB16 nc = keine_fillp(yago,x,y,c,*pix);
		*pix = nc;
	}
	//*mokou_pread16(yago,x,y) = c;
}

/*	--	fill functions	--	*/
void mokou_spr16(
	keine *src,keine *dst,SDL_Rect srcrect,
	mokou_sprattr attr
) {
	u8 xf = attr.flip&1;
	u8 yf = attr.flip>>1;
	s32 srcW = srcrect.w; s32 srcH = srcrect.h;
	s32 srcX = srcrect.x; s32 srcY = srcrect.y;
	s16 dx = (s16)(attr.pos>>8);
	s16 dy = (s16)(attr.pos&0xFFFF);
	// if flip, use end of line as start.
	s32 startX = xf ? (srcX+srcW) : (srcX);
	s32 startY = yf ? (srcY+srcH) : (srcY);
	s32 incX = xf ? -1 : 1;
	s32 incY = yf ? -1 : 1;
	// if flip, use start of line as end.
	s32 endX = xf ? (srcX) : (srcX+srcW);
	s32 endY = yf ? (srcY) : (srcY+srcH);

	for(s32 y=startY; y!=srcH; y += incY)
	{
		for(s32 x=startX; x!=srcW; x += incX)
		{
			RGB16 pix = mokou_pget16(src,x,y);
			if(pix!=0) mokou_pset16(dst,x,y,pix);
		}
	}
}

